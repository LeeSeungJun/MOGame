// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MOSpawner.h"
#include "MOSpawnManager.generated.h"

UCLASS(Blueprintable)
class MOGame_API AMOSpawnManager : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void InitSpawn(TAssetPtr<UDataTable>& SpawnTable);

public:
	UPROPERTY(EditDefaultsOnly)
	TAssetPtr<UDataTable> SpawnTable;

	UPROPERTY()
	TArray<AMOSpawner*> SpawnerList;

};
