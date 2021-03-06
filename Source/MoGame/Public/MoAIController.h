// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "MOAIController.generated.h"

/**

 */
UCLASS()
class MOGame_API AMOAIController : public AAIController
{
	GENERATED_UCLASS_BODY()

public:
	virtual void InitPlayerState() override;
	virtual void SetPawn(APawn* InPawn) override;
	virtual void Possess(APawn* Pawn) override;
	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn = true) override;

	virtual void Attack();
	void SetTargetEnemy(AMOCharacter* PawnTarget);
	class AMOCharacter* GetEnemy();

	UFUNCTION(BlueprintCallable, Category = "AI")
	void FindClosestEnemy();
	
	UFUNCTION(BlueprintCallable, Category = "PlayerController")
	virtual AMOCharacterAI* GetMOCharacterAI();

public:
	UPROPERTY()
	class AMOPlayerState* MOPlayerState;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName TargetEnemyKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName AITypeKeyName;

private:
	//Behavior Tree component reference
	UPROPERTY(Transient)
	class UBehaviorTreeComponent* BehaviorComponent;

	//Blackboard comp ref
	UPROPERTY(Transient)
	class UBlackboardComponent* BlackboardComponent;

	UPROPERTY()
	class AMOCharacterAI* MOCharacterAI;

	bool bIsFindEnemy;
};
