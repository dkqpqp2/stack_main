// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_WidgetComponent.h"
#include "Minigames/OBot/AI/UI/MG_UserWidget.h"

void UMG_WidgetComponent::InitWidget()
{
	Super::InitWidget();

	UMG_UserWidget* MG_UserWidget = Cast<UMG_UserWidget>(GetWidget());

	if (MG_UserWidget)
	{
		MG_UserWidget->SetOwningActor(GetOwner());
	}

}
