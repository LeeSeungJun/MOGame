// Fill out your copyright notice in the Description page of Project Settings.

#include "MOGame.h"
#include "MOBTService_AttackEnemy.h"

UMOBTService_AttackEnemy::UMOBTService_AttackEnemy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Run Attack Enemy";

	// all blueprint based nodes must create instances
	bCreateNodeInstance = true;
}

void UMOBTService_AttackEnemy::SetOwner(AActor* InActorOwner)
{
	AIController = Cast<AMOAIController>(InActorOwner);
}

void UMOBTService_AttackEnemy::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (AIController != nullptr)
	{
		AIController->Attack();
	}
}


