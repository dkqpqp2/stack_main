// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UWrapBox* PlayersListWrapBox;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* ReadyOrStartSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UMapSelectWidget* CurrentMapSelectWidget;

public:
	void SwitchSwitcherWidget(bool bIsReady);
};
