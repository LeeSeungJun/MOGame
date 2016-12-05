// Fill out your copyright notice in the Description page of Project Settings.

#include "MOGame.h"
#include "MOAnimNotifyState_Attack.h"

// Begin에서 End 사이에 Tick에 버튼 입력 및 버튼이 눌려진 상태라면 다음 공격이 실행
UMOAnimNotifyState_Attack::UMOAnimNotifyState_Attack(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

void UMOAnimNotifyState_Attack::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	if (MeshComp->GetWorld()->GetNetMode() == NM_DedicatedServer)
	{
		return;
	}

	bIsConsecutively = false;

	Received_NotifyBegin(MeshComp, Animation, TotalDuration);
}

void UMOAnimNotifyState_Attack::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	if (MeshComp->GetWorld()->GetNetMode() == NM_DedicatedServer)
	{
		return;
	}

	if (!bIsConsecutively)
	{
		AMOCharacter* Pawn = Cast<AMOCharacter>(MeshComp->GetOwner());
		if (Pawn && Pawn->AttackType > EAttackType::EAttack_None)
		{
			bIsConsecutively = true;
		}
	}
	Received_NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UMOAnimNotifyState_Attack::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	if (MeshComp->GetWorld()->GetNetMode() == NM_DedicatedServer)
	{
		return;
	}

	AMOCharacter* Pawn = Cast<AMOCharacter>(MeshComp->GetOwner());
	if (Pawn && bIsConsecutively && Pawn->AttackType > EAttackType::EAttack_None)
	{
		Pawn->Attack(EAttackType::EAttack_Consecutively, NextAnimation);
	}

	bIsConsecutively = false;
	Received_NotifyEnd(MeshComp, Animation);
}
