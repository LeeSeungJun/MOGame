// Fill out your copyright notice in the Description page of Project Settings.

#include "MOGame.h"
#include "MOAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AMOAIController::AMOAIController(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	BehaviorComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComponent"));
	BlackboardComponent = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComponent"));

	TargetEnemyKeyName = "TargetEnemy";
	bWantsPlayerState = true;
}

void AMOAIController::InitPlayerState()
{
	Super::InitPlayerState();
	MOPlayerState = Cast<AMOPlayerState>(PlayerState);
}

void AMOAIController::SetPawn(APawn* InPawn)
{
	AController::SetPawn(InPawn);

	MOCharacterAI = Cast<AMOCharacterAI>(InPawn);
}

void AMOAIController::Possess(APawn* Pawn)
{
	Super::Possess(Pawn);

	AMOCharacterAI* MOBot = Cast<AMOCharacterAI>(Pawn);

	//Initialize blackboard
	if (MOBot && MOBot->BehaviorTree && MOBot->BehaviorTree->BlackboardAsset)
	{
		BlackboardComponent->InitializeBlackboard(*MOBot->BehaviorTree->BlackboardAsset);
	}
	BehaviorComponent->StartTree(*MOBot->BehaviorTree);
}

void AMOAIController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
	if (bIsFindEnemy && !GetFocalPoint().IsZero() && GetPawn())
	{
		FVector Direction = GetFocalPoint() - GetPawn()->GetActorLocation();
		FRotator NewContorolRotation = Direction.Rotation();

		NewContorolRotation.Yaw = FRotator::ClampAxis(NewContorolRotation.Yaw);
		SetControlRotation(NewContorolRotation);

		if (GetPawn() && bUpdatePawn)
		{
			GetPawn()->FaceRotation(NewContorolRotation, DeltaTime);
		}
	}
}

void AMOAIController::FindClosestEnemy()
{
	AMOCharacterAI* MyBot = Cast<AMOCharacterAI>(GetPawn());

	if (MyBot != nullptr)
	{
		float ClosestEnemyDistSq = MyBot->DistanceSquared;
		AMOCharacter* ClosestEnemy = NULL;

		TArray<AActor*> ActorsArray;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMOCharacter::StaticClass(), ActorsArray);

		for (auto Character = ActorsArray.CreateIterator(); Character; ++Character)
		{
			AMOCharacter* MOEnemy = Cast<AMOCharacter>(*Character);
			if (MOEnemy != nullptr && MOEnemy->PlayerState != nullptr && !MOEnemy->PlayerState->bIsABot)
			{
				// 가장 가까운 적을 찾는다.
				const float DistSq = (MOEnemy->GetActorLocation() - MyBot->GetActorLocation()).SizeSquared();
				if (DistSq < ClosestEnemyDistSq)
				{
					ClosestEnemyDistSq = DistSq;
					ClosestEnemy = MOEnemy;
				}
			}
		}

		if (ClosestEnemy != nullptr)
		{
			bIsFindEnemy = true;
			SetTargetEnemy(ClosestEnemy);
		}
	}
}

void AMOAIController::Attack()
{
	AMOCharacter* Enemy = GetEnemy();
	AMOCharacterAI* MyBot = Cast<AMOCharacterAI>(GetPawn());
	if (Enemy != nullptr && MyBot != nullptr && !MyBot->bIsAttacking)
	{
		const float Dist = (Enemy->GetActorLocation() - MyBot->GetActorLocation()).Size2D();
		if (Dist < MyBot->AttackDistance)
		{
			MyBot->bIsAttacking = true;
			MyBot->Attack(EAttackType::EAttack_Attack);
		}
		else
		{
			MyBot->Attack(EAttackType::EAttack_None);
		}
	}
}

void AMOAIController::SetTargetEnemy(AMOCharacter* PawnTarget)
{
	if (BlackboardComponent)
	{
		BlackboardComponent->SetValueAsObject(TargetEnemyKeyName, PawnTarget);
		SetFocus(PawnTarget);
	}
}

AMOCharacter* AMOAIController::GetEnemy()
{
	if (BlackboardComponent)
	{
		return Cast<AMOCharacter>(BlackboardComponent->GetValueAsObject(TargetEnemyKeyName));
	}
	return NULL;
}

AMOCharacterAI* AMOAIController::GetMOCharacterAI()
{
	return MOCharacterAI;
}
