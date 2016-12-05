// Fill out your copyright notice in the Description page of Project Settings.

#include "MOGame.h"
#include "MOSpawner.h"


// Sets default values
AMOSpawner::AMOSpawner(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	SpawnerArrow = ObjectInitializer.CreateDefaultSubobject<UArrowComponent>(this, TEXT("SpawnerArrow"));
#endif
}

AMOCharacter* AMOSpawner::Spawn(UClass* SpawnClass)
{
	return GetWorld()->SpawnActor<AMOCharacter>(SpawnClass, GetActorLocation(), GetActorRotation());
}