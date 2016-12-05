// Fill out your copyright notice in the Description page of Project Settings.

#include "MOGame.h"
#include "MOBTTask_MoveArea.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

UMOBTTask_MoveArea::UMOBTTask_MoveArea(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	NodeName = "Move Area";
}

EBTNodeResult::Type UMOBTTask_MoveArea::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UNavigationSystem* NavigationSystem = UNavigationSystem::GetCurrent(GetWorld());
	AMOCharacterAI* MyCharacter = Cast<AMOCharacterAI>(OwnerComp.GetAIOwner()->GetPawn());

	if (NavigationSystem != nullptr && MyCharacter != nullptr)
	{
		FNavLocation RandomPointIn;
		if (NavigationSystem->GetRandomPointInNavigableRadius(MyCharacter->GetActorLocation(), MyCharacter->AreaRadius, RandomPointIn))
		{
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(GetSelectedBlackboardKey(), RandomPointIn.Location);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}


