// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MatchCountdownText; // 줄어드는거

	//UPROPERTY(meta = (BindWidget))
	//class UTextBlock* MatchTimerText; //늘어나는 거
	
	UPROPERTY(meta = (BindWidget))
	class UItemSlotWidget* ItemSlot;
};
