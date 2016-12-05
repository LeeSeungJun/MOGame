// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MOWeapon.h"
#include "MOWeapon_Melee.generated.h"

/**
 * 
 */
UCLASS()
class MOGame_API AMOWeapon_Melee : public AMOWeapon
{
	GENERATED_UCLASS_BODY()

public:
	virtual void DetachFromOwner() override;

public:
	/** particle component for muzzle flash */
	UPROPERTY(EditAnywhere, Category = "Component")
	class UParticleSystemComponent* PSCSwing;

	/** socket to attach weapon to hands; if None, then the hands are hidden */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName SwingSocketName;
};
