// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "MOUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class MOGame_API UMOUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UMOUserWidget(const FObjectInitializer& ObjectInitializer);

	virtual void AttachToController(AMOPlayerController* Controller);
	virtual void DetachFromController();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WidgetInfo")
	FString WidgetName;

	UPROPERTY(EditAnywhere, Category = "DepthOrder")
	int16 UIDepth;

	UPROPERTY(BlueprintReadOnly, Category = "Controller")
	AMOPlayerController* OwnerController;
};
