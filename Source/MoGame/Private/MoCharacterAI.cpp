// Fill out your copyright notice in the Description page of Project Settings.

#include "MOGame.h"
#include "MOCharacterAI.h"

AMOCharacterAI::AMOCharacterAI(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.DoNotCreateDefaultSubobject(TEXT("CharacterCameraComp")).DoNotCreateDefaultSubobject(TEXT("CameraCapsuleComp")))
{
	DistanceSquared = 30000.f;
	AttackDistance = 150.f;
	AreaRadius = 200.f;
	FiringDelay = 0.5f;

	AIControllerClass = AMOAIController::StaticClass();
}

void AMOCharacterAI::PossessedBy(AController* NewController)
{
	AMOPlayerState* MOPS = Cast<AMOPlayerState>(PlayerState);
	if (MOPS != nullptr)
	{
		MOPS->bIsABot = true;
	}

	Super::PossessedBy(NewController);
}

//void AMOCharacterAI::NotifyActorBeginOverlap(AActor* OtherActor)
//{
//	Super::NotifyActorBeginOverlap(OtherActor);
//
//	if (OtherActor->IsA(AMOCharacter::StaticClass()) && PlayerState != nullptr && !PlayerState->bIsABot)
//	{
//		UGameplayStatics::ApplyDamage(OtherActor, Damage, NULL, this, UDamageType::StaticClass());
//	}
//}

void AMOCharacterAI::FaceRotation(FRotator NewControlRotation, float DeltaTime)
{
	FRotator CurrentRotator = FMath::RInterpTo(GetActorRotation(), NewControlRotation, DeltaTime, 8.f);

	Super::FaceRotation(NewControlRotation, DeltaTime);
}

void AMOCharacterAI::Attack(EAttackType EType, int AnimationState)
{
	AttackType = EType;
	if (EType == EAttackType::EAttack_Attack)
	{
		PlayAnimMontage(AttackMontageList[AnimationState]);
	}
	else if (EType == EAttackType::EAttack_Consecutively)
	{
		PlayAnimMontage(AttackMontageList[AnimationState]);
		AttackType = EAttackType::EAttack_Attack;

		if (bIsAttacking)
		{
			FTimerHandle AttackTimerHandle;
			GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AMOCharacterAI::OnAttackEnd, AttackMontageList[AnimationState]->GetPlayLength() + FiringDelay, false);
			AttackType = EAttackType::EAttack_None;
		}
	}
}

void AMOCharacterAI::OnAttackEnd()
{
	bIsAttacking = false;
}