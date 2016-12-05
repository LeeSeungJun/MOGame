// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "MOAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MOGame_API UMOAnimInstance : public UAnimInstance
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "Move")
	float Speed;

	UPROPERTY(BlueprintReadWrite, Category = "Move")
	float Direction;

	UPROPERTY(BlueprintReadWrite, Category = "Move")
	bool bBlocking;
	
};
