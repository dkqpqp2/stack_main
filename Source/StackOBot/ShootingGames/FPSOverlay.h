// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPSOverlay.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API UFPSOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar_1;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar_2;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar_3;

	UPROPERTY(meta = (BindWidget))	
	class UTextBlock* HealthText;
};
