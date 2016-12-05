// Fill out your copyright notice in the Description page of Project Settings.

#include "MOGame.h"
#include "MOPlayerController.h"

AMOPlayerController::AMOPlayerController(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	PlayerCameraManagerClass = APlayerCameraManager::StaticClass();
}

void AMOPlayerController::InitPlayerState()
{
	Super::InitPlayerState();
	MOPlayerState = Cast<AMOPlayerState>(PlayerState);
}

void AMOPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

#if !UE_SERVER
	for (auto WidgetAsset : DisplayWidgetList)
	{
		UMOUserWidget* NewWidget = CreateWidget<UMOUserWidget>(this, WidgetAsset);
		if (NewWidget != nullptr)
		{
			NewWidget->AttachToController(this);
		}
	}
#endif
}

void AMOPlayerController::SetPawn(APawn* InPawn)
{
	if (InPawn == nullptr)
	{
		// Attempt to move the PC to the current camera location if no pawn was specified
		const FVector NewLocation = (PlayerCameraManager != nullptr) ? PlayerCameraManager->GetCameraLocation() : GetSpawnLocation();
		SetSpawnLocation(NewLocation);
	}

	AController::SetPawn(InPawn);

	MOCharacter = Cast<AMOCharacter>(InPawn);
}

void AMOPlayerController::InitInputSystem()
{
	Super::InitInputSystem();
}

void AMOPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &AMOPlayerController::MoveForward);
	InputComponent->BindAxis("MoveBackward", this, &AMOPlayerController::MoveBackward);
	InputComponent->BindAxis("MoveLeft", this, &AMOPlayerController::MoveLeft);
	InputComponent->BindAxis("MoveRight", this, &AMOPlayerController::MoveRight);
}

void AMOPlayerController::MoveForward(float Value)
{
	if (Value != 0.0f && MOCharacter != nullptr)
	{
		MOCharacter->MoveForward(Value);
		StickMoveDirection.X = Value;
		UpdateInputRotate();
	}
}

void AMOPlayerController::MoveBackward(float Value)
{
	MoveForward(Value * -1);
}

void AMOPlayerController::MoveLeft(float Value)
{
	MoveRight(Value * -1);
}

void AMOPlayerController::MoveRight(float Value)
{
	if (Value != 0.0f && MOCharacter != nullptr)
	{
		MOCharacter->MoveRight(Value);
		StickMoveDirection.Y = Value;
		UpdateInputRotate();
	}
}
void AMOPlayerController::Attack(EAttackType EType)
{
	if (MOCharacter != nullptr)
	{
		MOCharacter->Attack(EType);
	}
}

void AMOPlayerController::Block(bool BlockState)
{
	if (MOCharacter != nullptr)
	{
		SetIgnoreMoveInput(BlockState);
		SetIgnoreLookInput(BlockState);
		MOCharacter->GetMOAnimInstance()->bBlocking = BlockState;
	}
}

void AMOPlayerController::UpdateInputRotate()
{
	if (GetWorld()->GetNetMode() != NM_DedicatedServer)
	{
		FRotator PredictionRotator(GetControlRotation());
		UpdatePredictionRotator(PredictionRotator);
		SetControlRotation(PredictionRotator);
	}
}

void AMOPlayerController::UpdatePredictionRotator(FRotator& PredictionRotator)
{
	// Input Direction은 2차원 xy 절대좌표이므로 Camera Default yaw에 맞춰 회전시킴
	StickMoveDirection.Normalize();
	FVector StickToWorld = StickMoveDirection;
	MOCharacter->GetRotateVectorByCamera(StickToWorld);

	PredictionRotator = StickToWorld.Rotation();
}

void AMOPlayerController::ClientSetHUD_Implementation(TSubclassOf<class AHUD> NewHUDClass)
{
	Super::ClientSetHUD_Implementation(NewHUDClass);

	if (MyHUD != nullptr)
	{
		MyMOHUD = Cast<AMOHUD>(MyHUD);
	}
}

AMOCharacter* AMOPlayerController::GetMOCharacter()
{
	return MOCharacter;
}
