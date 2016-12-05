// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "MOSpawnManager.h"
#include "MOGameGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MOGAME_API AMOGameGameMode : public AGameMode
{
	GENERATED_UCLASS_BODY()

public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void GenericPlayerInitialization(AController* C) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<TSubclassOf<class AMOInventory>> GameModeInventory;

	UPROPERTY()
	class AMOSpawnManager* SpawnManager;
};
