// Fill out your copyright notice in the Description page of Project Settings.

#include "MOGame.h"
#include "MOWeapon_Melee.h"

AMOWeapon_Melee::AMOWeapon_Melee(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	MeleeCollision = ObjectInitializer.CreateOptionalDefaultSubobject<UCapsuleComponent>(this, TEXT("MeleeCollision"));
	if (MeleeCollision != nullptr && WeaponMeshComponent != nullptr)
	{
		MeleeCollision->SetupAttachment(WeaponMeshComponent);
		MeleeCollision->OnComponentBeginOverlap.AddDynamic(this, &AMOWeapon::OnOverlapBegin);
		MeleeCollision->SetCastShadow(false);

		SwingSocketName = FName(TEXT("Center"));
		PSCSwing = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("PSCSwing"));
		if (PSCSwing != nullptr)
		{
			PSCSwing->SetupAttachment(WeaponMeshComponent, SwingSocketName);
			PSCSwing->SetCastShadow(false);
		}
	}
	//bIsDrawWeaponCollision = true;
}

void AMOWeapon_Melee::DetachFromOwner()
{
	if (PSCSwing != nullptr)
	{
		PSCSwing->DeactivateSystem();
	}

	Super::DetachFromOwner();
}