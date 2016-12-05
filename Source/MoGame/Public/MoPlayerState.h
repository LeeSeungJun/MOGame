// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "MOPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MOGame_API AMOPlayerState : public APlayerState
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION()
	class AMOCharacter* GetMOCharacter();

	// �κ� ������ ������ ������ ����
	virtual void ApplyItem(AMOCharacter* OwnerCharacter = nullptr);
};
