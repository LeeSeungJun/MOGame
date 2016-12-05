// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MOAIController.h"
#include "MOAIController_Magic.generated.h"

/**
 * 
 */
UCLASS()
class MOGame_API AMOAIController_Magic : public AMOAIController
{
	GENERATED_UCLASS_BODY()

public:
	virtual void Attack() override;
};
