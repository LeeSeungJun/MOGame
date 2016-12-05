// Fill out your copyright notice in the Description page of Project Settings.

#include "MOGame.h"
#include "MOAIController_Magic.h"

AMOAIController_Magic::AMOAIController_Magic(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

void AMOAIController_Magic::Attack()
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
	}
}
