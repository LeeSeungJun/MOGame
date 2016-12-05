// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "MOCharacter.generated.h"

USTRUCT()
struct FWeaponInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	class AMOWeapon* MOWeapon;

	UPROPERTY()
	FName SocketName;
};

UCLASS()
class MOGame_API AMOCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

public:
	virtual void PossessedBy(AController* NewController) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void MoveForward(float Value);
	virtual void MoveRight(float Value);

	virtual void Attack(EAttackType EType, int AnimationState = 0);

	void GetCameraLookatVector(FVector& Lookat, FRotator& CameraRotate);

	// 절대 x,y 이동 Vector를 Camera시점으로 재 변환
	void GetRotateVectorByCamera(FVector& RotateVector);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual bool AddInventory(AMOInventory* InvToAdd, bool bAutoActivate);

	//UFUNCTION(BlueprintCallable, Category = "Weapon")
	//virtual void RemoveInventory(AMOInventory* InvToRemove);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void StartFire(FName Socket);

	virtual void AddDefaultInventory(TArray<TSubclassOf<AMOInventory>> DefaultInventoryToAdd);

	virtual void NotifyTakeHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser);

	virtual void OnDied(float KillingDamage, struct FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser);

	void OnDiedAnimationEnd();
	UFUNCTION(BlueprintCallable, Category = "Character")
	FORCEINLINE UMOAnimInstance* GetMOAnimInstance()
	{
		return MOAnimInstance != nullptr ? MOAnimInstance : nullptr;
	}

	UFUNCTION()
	FORCEINLINE bool bIsBot() const
	{
		return (PlayerState != nullptr) ? PlayerState->bIsABot : false;
	}
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class USkeletalMeshComponent* CapeSkeletalMeshComponent;

	/** Camera를 둘러 싸는 Component - 벽에 부딪힐 Camera 처리 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraCapsuleComponent;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CharacterCameraComponent;

	UPROPERTY()
	FVector CameraLookat;

	UPROPERTY()
	FRotator CameraRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	TArray<class UAnimMontage*> AttackMontageList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* FlinchMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
	class UAnimMontage* BlockMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<TSubclassOf<class AMOInventory>> CharacterInventory;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	class AMOInventory* InventoryList;

	UPROPERTY()
	TArray<FWeaponInfo> MOWeaponList;

	// AI만 Firing Delay 적용
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float FiringDelay;

	UPROPERTY()
	bool bIsAttacking;

	EAttackType AttackType;

	UPROPERTY(BlueprintReadWrite, Category = "Pawn")
	int32 Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn")
	int32 HealthMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	float HealthActorExtent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	float HealthOffsetY;

	UPROPERTY()
	class AMOWeapon* CurrentWeapon;

protected:
	UPROPERTY()
	class UMOAnimInstance* MOAnimInstance;

	float OrgMaxWalkSpeed;
};
