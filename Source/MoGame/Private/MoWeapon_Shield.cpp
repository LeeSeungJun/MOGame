// Fill out your copyright notice in the Description page of Project Settings.

#include "MOGame.h"
#include "MOWeapon_Shield.h"

AMOWeapon_Shield::AMOWeapon_Shield(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.DoNotCreateDefaultSubobject("WeaponRMeshComp"))
{
	WeaponMeshComponent = ObjectInitializer.CreateOptionalDefaultSubobject<UStaticMeshComponent>(this, TEXT("WeaponLMeshComp"));
	if (WeaponMeshComponent != nullptr)
	{
		WeaponMeshComponent->SetupAttachment(RootComponent);
		WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponMeshComponent->SetCastShadow(false);
	}

	HandsAttachSocketName = TEXT("Weapon_L");
}