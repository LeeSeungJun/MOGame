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
	// �κ񿡼� InGame �������� �߰��� ItemBase ����
	// ItemBase�� ���� Item ���� [ Weapon / Equipment / Accessory ��� ]
	// �ϴ� ������ DefaultInventory Item ����

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
