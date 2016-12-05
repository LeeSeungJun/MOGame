// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MOInventory.generated.h"

UCLASS(Blueprintable, Abstract, notplaceable)
class MOGame_API AMOInventory : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:
	AMOCharacter* GetMOOwner() const
	{
		checkSlow(MOOwner == GetMOOwner());
		return MOOwner;
	}

	virtual void GivenTo(AMOCharacter* NewOwner, bool bAutoActivate);

protected:
	UFUNCTION(Client, Reliable)
	void ClientGivenTo(APawn* NewInstigator, bool bAutoActivate);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	class AMOCharacter* MOOwner;

};
