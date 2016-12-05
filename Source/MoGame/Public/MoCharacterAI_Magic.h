// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MOCharacterAI.h"
#include "MOCharacterAI_Magic.generated.h"

/**
 * 
 */
UCLASS()
class MOGame_API AMOCharacterAI_Magic : public AMOCharacterAI
{
	GENERATED_UCLASS_BODY()

public:
	virtual void Attack(EAttackType EType, int AnimationState = 0) override;
	virtual void OnAttackEnd() override;
};
