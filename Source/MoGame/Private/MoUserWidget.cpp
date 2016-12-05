// Fill out your copyright notice in the Description page of Project Settings.

#include "MOGame.h"
#include "MOUserWidget.h"

UMOUserWidget::UMOUserWidget(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

void UMOUserWidget::AttachToController(AMOPlayerController* Controller)
{
	if (GetWorld()->GetNetMode() == NM_DedicatedServer)
		return;

	if (Controller && Controller->IsLocalController())
	{
		OwnerController = Controller;
		AddToViewport(UIDepth);
	}
}


void UMOUserWidget::DetachFromController()
{
	if (GetWorld()->GetNetMode() == NM_DedicatedServer)
		return;

	if (OwnerController)
	{
		OwnerController = nullptr;
	}
}


