// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTService.h"
#include "MOBTService_AttackEnemy.generated.h"

/**
 * 
 */
UCLASS()
class MOGame_API UMOBTService_AttackEnemy : public UBTService
{
	GENERATED_UCLASS_BODY()

public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void SetOwner(AActor* ActorOwner) override;

protected:
	AMOAIController* AIController;
};
