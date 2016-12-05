// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "MOPlayerController.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EAttackType : uint8
{
	EAttack_None,
	EAttack_Attack,
	EAttack_Consecutively,
};

UCLASS()
class MOGame_API AMOPlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()

public:
	virtual void InitPlayerState() override;
	virtual void ReceivedPlayer() override;
	virtual void SetPawn(APawn* InPawn) override;

	// Input
	virtual void SetupInputComponent() override;
	virtual void InitInputSystem() override;

	virtual void MoveForward(float Value);
	virtual void MoveBackward(float Value);
	virtual void MoveLeft(float Value);
	virtual void MoveRight(float Value);

	virtual void UpdateInputRotate();
	virtual void UpdatePredictionRotator(FRotator& PredictionRotator);

	UFUNCTION(BlueprintCallable, Category = "Event")
	virtual void Attack(EAttackType EType);

	UFUNCTION(BlueprintCallable, Category = "Event")
	virtual void Block(bool BlockState);

	UFUNCTION(BlueprintCallable, Category = "HUD", Reliable, Client)
	void ClientSetHUD(TSubclassOf<class AHUD> NewHUDClass);

	UFUNCTION(BlueprintCallable, Category = PlayerController)
	virtual AMOCharacter* GetMOCharacter();

private:
	UPROPERTY()
	class AMOCharacter* MOCharacter;

	UPROPERTY()
	class AMOHUD* MyMOHUD;

	UPROPERTY()
	TSubclassOf<class AHUD> HUDClass;

public:
	UPROPERTY()
	class AMOPlayerState* MOPlayerState;

	UPROPERTY()
	FVector StickMoveDirection;

	// 배열자리가 Depth값이다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMG Widget")
	TArray<TSubclassOf<class UMOUserWidget>> DisplayWidgetList;
};
