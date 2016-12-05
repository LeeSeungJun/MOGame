// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MOSpawner.generated.h"

UENUM(BlueprintType)
enum class ESpawnType : uint8
{
	Melee,
	Magic,
	None,
};

USTRUCT(Blueprintable)
struct FMOSpawnInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	// �߰� ������ ������ ���� �߰�!
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ESpawnType SpawnType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TAssetSubclassOf<AMOCharacter> SpawnActorAsset;
};

UCLASS(Blueprintable)
class MOGame_API AMOSpawner : public AActor
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = Spawn)
	virtual AMOCharacter* Spawn(UClass* SpawnClass);

public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ESpawnType SpawnType;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UArrowComponent* SpawnerArrow;
#endif
};
