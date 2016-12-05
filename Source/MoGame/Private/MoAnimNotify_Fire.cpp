// Fill out your copyright notice in the Description page of Project Settings.

#include "MOGame.h"
#include "MOAnimNotify_Fire.h"

UMOAnimNotify_Fire::UMOAnimNotify_Fire(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

void UMOAnimNotify_Fire::Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation)
{
	AMOCharacter* Pawn = Cast<AMOCharacter>(MeshComp->GetOwner());
	if (Pawn != nullptr)
	{
		Pawn->StartFire(SocketName);
	}
}
