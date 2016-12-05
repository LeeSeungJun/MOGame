// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "MOAnimNotify_Fire.generated.h"

/**
 * 
 */
UCLASS()
class MOGame_API UMOAnimNotify_Fire : public UAnimNotify
{
	GENERATED_UCLASS_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
	FName SocketName;
};
