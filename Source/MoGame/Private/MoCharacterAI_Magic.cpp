// Fill out your copyright notice in the Description page of Project Settings.

#include "MOGame.h"
#include "MOCharacterAI_Magic.h"

AMOCharacterAI_Magic::AMOCharacterAI_Magic(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.DoNotCreateDefaultSubobject(TEXT("MOtackCollisionSphere")))
{
	AIControllerClass = AMOCharacterAI_Magic::StaticClass();
}

void AMOCharacterAI_Magic::Attack(EAttackType EType, int AnimationState)
{
	if (AttackMontageList[AnimationState] != nullptr && EType == EAttackType::EAttack_Attack && !MOAnimInstance->Montage_IsPlaying(AttackMontageList[AnimationState]))
	{
		PlayAnimMontage(AttackMontageList[AnimationState]);

		if (bIsAttacking)
		{
			FTimerHandle AttackTimerHandle;
			GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AMOCharacterAI_Magic::OnAttackEnd, AttackMontageList[AnimationState]->GetPlayLength() + FiringDelay, false);
			AttackType = EAttackType::EAttack_None;
			GetCharacterMovement()->MaxWalkSpeed = 0.f;
		}
	}
}

void AMOCharacterAI_Magic::OnAttackEnd()
{
	bIsAttacking = false;
	GetCharacterMovement()->MaxWalkSpeed = OrgMaxWalkSpeed;
}