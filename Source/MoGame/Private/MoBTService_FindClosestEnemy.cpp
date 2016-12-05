// Fill out your copyright notice in the Description page of Project Settings.

#include "MOGame.h"
#include "MOBTService_FindClosestEnemy.h"

UMOBTService_FindClosestEnemy::UMOBTService_FindClosestEnemy(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	NodeName = "Run Find Closest Enemy";

	// all blueprint based nodes must create instances
	bCreateNodeInstance = true;
}

void UMOBTService_FindClosestEnemy::SetOwner(AActor* InActorOwner)
{
	AIController = Cast<AMOAIController>(InActorOwner);
}

void UMOBTService_FindClosestEnemy::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (AIController != nullptr)
	{
		AIController->FindClosestEnemy();
	}
}