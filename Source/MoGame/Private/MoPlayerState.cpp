// Fill out your copyright notice in the Description page of Project Settings.

#include "MOGame.h"
#include "MOPlayerState.h"
#include "MOGameGameMode.h"

AMOPlayerState::AMOPlayerState(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

AMOCharacter* AMOPlayerState::GetMOCharacter()
{
	AController* Controller = Cast<AController>(GetOwner());
	if (Controller != nullptr)
	{
		AMOCharacter* Character = Cast<AMOCharacter>(Controller->GetPawn());
		if (Character != nullptr)
		{
			return Character;
		}
	}
	return nullptr;
}

void AMOPlayerState::ApplyItem(AMOCharacter* OwnerCharacter)
{
	// 로비에서 InGame 아이템이 추가시 ItemBase 생성
	// ItemBase를 따라서 Item 적용 [ Weapon / Equipment / Accessory 등등 ]
	// 일단 강제로 DefaultInventory Item 적용

	if (OwnerCharacter == nullptr)
	{
		OwnerCharacter = GetMOCharacter();
	}

	AMOGameGameMode* MOGameMode = GetWorld()->GetAuthGameMode<AMOGameGameMode>();
	if (OwnerCharacter && OwnerCharacter->CharacterInventory.Num() != 0 && MOGameMode)
	{
		OwnerCharacter->AddDefaultInventory(MOGameMode->GameModeInventory);
	}
}
