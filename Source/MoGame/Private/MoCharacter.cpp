// Fill out your copyright notice in the Description page of Project Settings.

#include "MOGame.h"
#include "MOCharacter.h"


// Sets default values
AMOCharacter::AMOCharacter(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	if (GetMesh())
	{
		GetMesh()->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->bEnablePhysicsOnDedicatedServer = true; // needed for feign death; death ragdoll shouldn't be invoked on server

		CapeSkeletalMeshComponent = ObjectInitializer.CreateOptionalDefaultSubobject<USkeletalMeshComponent>(this, TEXT("CapeSkeletalMeshComp"));
		if (CapeSkeletalMeshComponent != nullptr)
		{
			CapeSkeletalMeshComponent->SetupAttachment(GetMesh(), TEXT("Cape"));
			CapeSkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			CapeSkeletalMeshComponent->SetCastShadow(false);
		}
	}

	CameraCapsuleComponent = ObjectInitializer.CreateOptionalDefaultSubobject<USpringArmComponent>(this, TEXT("CameraCapsuleComp"));
	if (CameraCapsuleComponent)
	{
		CameraCapsuleComponent->SetupAttachment(GetCapsuleComponent());
		CameraCapsuleComponent->bUsePawnControlRotation = false; // Rotate the arm based on the controller

																 // Create a follow camera
		CharacterCameraComponent = ObjectInitializer.CreateOptionalDefaultSubobject<UCameraComponent>(this, TEXT("CharacterCameraComp"));
		if (CharacterCameraComponent != nullptr)
		{
			// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
			CharacterCameraComponent->SetupAttachment(CameraCapsuleComponent, USpringArmComponent::SocketName);
			CharacterCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
		}
	}

	HealthActorExtent = 45.f;
	HealthOffsetY = 100.f;
	Health = HealthMax;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AMOCharacter::PossessedBy(AController* NewController)
{
	// TODO: shouldn't base class do this? APawn::Unpossessed() still does SetOwner(NULL)...
	SetOwner(NewController);

	Super::PossessedBy(NewController);

	AMOPlayerState* MOPS = Cast<AMOPlayerState>(PlayerState);
	if (MOPS != nullptr)
	{
		MOPS->ApplyItem(this);
	}
}

// Called when the game starts or when spawned
void AMOCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	MOAnimInstance = Cast<UMOAnimInstance>(GetMesh()->GetAnimInstance());

	GetCameraLookatVector(CameraLookat, CameraRotation);

	OrgMaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

void AMOCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation(0.f, CameraLookat.Rotation().Yaw, 0.f);
		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Value);
	}
}

void AMOCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation(0.f, CameraLookat.Rotation().Yaw, 0.f);
		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
	}
}

void AMOCharacter::Attack(EAttackType EType, int AnimationState)
{
	AttackType = EType;
	if (EType == EAttackType::EAttack_Attack && !IsPlayingRootMotion())
	{
		PlayAnimMontage(AttackMontageList[AnimationState]);
	}
	else if (EType == EAttackType::EAttack_Consecutively)
	{
		PlayAnimMontage(AttackMontageList[AnimationState]);
		AttackType = EAttackType::EAttack_Attack;
	}
}

void AMOCharacter::StartFire(FName Socket)
{
	for (int Index = 0; Index < MOWeaponList.Num(); ++Index)
	{
		if (MOWeaponList[Index].SocketName == Socket)
		{
			CurrentWeapon = MOWeaponList[Index].MOWeapon;
			if (CurrentWeapon != nullptr)
			{
				CurrentWeapon->FireShot();
			}
			return;
		}
	}
}

void AMOCharacter::GetCameraLookatVector(FVector& Lookat, FRotator& CameraRotate)
{
	if (CameraCapsuleComponent == nullptr)
	{
		Lookat = GetActorLocation();
		CameraRotate = GetActorRotation();
		return;
	}

	CameraRotate = FRotator(0.f, CameraCapsuleComponent->GetRelativeTransform().GetRotation().Rotator().Yaw, 0.f);
	Lookat = CameraRotate.RotateVector(FVector::ForwardVector);
}

void AMOCharacter::GetRotateVectorByCamera(FVector& RotateVector)
{
	RotateVector = FRotationMatrix(CameraRotation).TransformVector(RotateVector);
}

bool AMOCharacter::AddInventory(AMOInventory* InvToAdd, bool bAutoActivate)
{
	if (InvToAdd != NULL && !InvToAdd->IsPendingKillPending())
	{
		if (InventoryList == NULL)
		{
			InventoryList = InvToAdd;
		}
		InvToAdd->GivenTo(this, bAutoActivate);
		return true;
	}
	return false;
}

void AMOCharacter::AddDefaultInventory(TArray<TSubclassOf<AMOInventory>> DefaultInventoryToAdd)
{
	// 캐릭터가 가지고 있는 인벤토리
	for (int32 i = 0; i< CharacterInventory.Num(); i++)
	{
		AddInventory(GetWorld()->SpawnActor<AMOInventory>(CharacterInventory[i], FVector(0.0f), FRotator(0, 0, 0)), true);
	}

	// 모드 전용 인벤토리
	for (int32 i = 0; i< DefaultInventoryToAdd.Num(); i++)
	{
		AddInventory(GetWorld()->SpawnActor<AMOInventory>(DefaultInventoryToAdd[i], FVector(0.0f), FRotator(0, 0, 0)), true);
	}
}

float AMOCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Health <= 0.f)
	{
		return 0.f;
	}

	if (GetMOAnimInstance()->bBlocking)
	{
		PlayAnimMontage(BlockMontage);
		return 0.f;
	}

	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0.f)
	{
		Health -= ActualDamage;
	}

	if (Health <= 0)
	{
		OnDied(ActualDamage, DamageEvent, EventInstigator, DamageCauser);
	}
	else
	{
		NotifyTakeHit(ActualDamage, DamageEvent, EventInstigator ? EventInstigator->GetPawn() : NULL, DamageCauser);
	}

	return ActualDamage;
}

void AMOCharacter::NotifyTakeHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser)
{
	PlayAnimMontage(FlinchMontage);

	if (DamageTaken > 0.f)
	{
		ApplyDamageMomentum(DamageTaken, DamageEvent, PawnInstigator, DamageCauser);
	}
}

void AMOCharacter::OnDied(float KillingDamage, struct FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser)
{
	Health = FMath::Min(0, Health);

	UDamageType const* const DamageType =
		DamageEvent.DamageTypeClass ? Cast<const UDamageType>(DamageEvent.DamageTypeClass->GetDefaultObject()) :
		GetDefault<UDamageType>();

	Killer = GetDamageInstigator(Killer, *DamageType);

	GetWorldTimerManager().ClearAllTimersForObject(this);

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->BodyInstance.SetResponseToChannel(ECC_Pawn, ECR_Ignore);
		GetCapsuleComponent()->BodyInstance.SetResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->DisableMovement();
	}

	if (Controller != NULL)
	{
		Controller->UnPossess();
	}

	float DeathAnimDuration = PlayAnimMontage(DeathMontage);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMOCharacter::OnDiedAnimationEnd, DeathAnimDuration, false);
}

void AMOCharacter::OnDiedAnimationEnd()
{
	for (int Index = 0; Index < MOWeaponList.Num(); ++Index)
	{
		MOWeaponList[Index].MOWeapon->DetachFromOwner();
	}
	SetActorHiddenInGame(true);
	SetLifeSpan(0.1f);
}