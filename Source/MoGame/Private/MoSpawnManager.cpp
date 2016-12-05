// Fill out your copyright notice in the Description page of Project Settings.

#include "MOGame.h"
#include "MOSpawnManager.h"


// Sets default values
AMOSpawnManager::AMOSpawnManager(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

// Called when the game starts or when spawned
void AMOSpawnManager::BeginPlay()
{
	Super::BeginPlay();

	InitSpawn(SpawnTable);
}

void AMOSpawnManager::InitSpawn(TAssetPtr<UDataTable>& Table)
{
	UDataTable* SpawnTableAsset = (Table.IsNull()) ? nullptr : Table.LoadSynchronous();

	if (SpawnTableAsset == nullptr)
	{
		return;
	}

	// 월드에 있는 AMOSpawner가지고 와서 SpawnerList넣어준다.
	TArray<AActor*> ActorsArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMOSpawner::StaticClass(), ActorsArray);

	for (auto Spawne = ActorsArray.CreateIterator(); Spawne; ++Spawne)
	{
		AMOSpawner* MOSpawne = Cast<AMOSpawner>(*Spawne);
		if (MOSpawne != nullptr)
		{
			for (auto TableKey : SpawnTableAsset->GetRowNames())
			{
				FMOSpawnInfo* SpawnInfo = SpawnTable->FindRow<FMOSpawnInfo>(TableKey, TEXT("MOSpawnInfo"));
				if (SpawnInfo->SpawnType == MOSpawne->SpawnType)
				{
					if (SpawnInfo->SpawnActorAsset.IsNull() == false)
					{
						const FStringAssetReference& AssetReference = SpawnInfo->SpawnActorAsset.ToStringReference();
						AMOCharacterAI* CharacterAI = Cast<AMOCharacterAI>(MOSpawne->Spawn(Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *AssetReference.ToString(), nullptr, LOAD_NoWarn))));

						if (CharacterAI->Controller == nullptr)
						{
							CharacterAI->SpawnDefaultController();
						}
					}
					break;
				}
			}
		}
	}
}
