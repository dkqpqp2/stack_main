// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReadyButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API UReadyButtonWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* ReadyButton;

	UFUNCTION()
	void OnReadyButtonClicked();
};
