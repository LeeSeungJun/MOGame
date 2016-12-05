// Fill out your copyright notice in the Description page of Project Settings.

#include "MOGame.h"
#include "MOInventory.h"


// Sets default values
AMOInventory::AMOInventory(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent, USceneComponent>(this, TEXT("DummyRoot"), false);

	PrimaryActorTick.bCanEverTick = true;
}

void AMOInventory::GivenTo(AMOCharacter* NewOwner, bool bAutoActivate)
{
	Instigator = NewOwner;
	MOOwner = NewOwner;
	SetOwner(NewOwner);
	PrimaryActorTick.AddPrerequisite(Instigator, Instigator->PrimaryActorTick);
	//ClientGivenTo(Instigator, bAutoActivate);
}

void AMOInventory::ClientGivenTo_Implementation(APawn* NewInstigator, bool bAutoActivate)
{
	if (NewInstigator != nullptr)
	{
		Instigator = NewInstigator;
	}
}