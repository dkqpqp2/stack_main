// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"
#include "Components/WidgetSwitcher.h"

void ULobbyWidget::SwitchSwitcherWidget(bool bHasAuthority)
{
	if (bHasAuthority)
	{
		ReadyOrStartSwitcher->SetActiveWidgetIndex(1);
	}
	else
	{
		ReadyOrStartSwitcher->SetActiveWidgetIndex(0);
	}
}
