// Fill out your copyright notice in the Description page of Project Settings.

#include "MOGame.h"
#include "MOGameGameMode.h"

AMOGameGameMode::AMOGameGameMode(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

// Parse options for this game...
void AMOGameGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	for (TActorIterator<AMOSpawnManager> It(GetWorld()); It; ++It)
	{
		// 무조건 한개만 존재
		SpawnManager = *It;
		break;
	}
}

void AMOGameGameMode::GenericPlayerInitialization(AController* C)
{
	AMOPlayerController* PC = Cast<AMOPlayerController>(C);
	if (PC != NULL)
	{
		// Notify the game that we can now be muted and mute others
		UpdateGameplayMuteList(PC);
		
		PC->ClientSetHUD(HUDClass);

		ReplicateStreamingStatus(PC);
	}
}