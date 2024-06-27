// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InBagAmmoWidget.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API UInBagAmmoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AmmosText;
	
	void SetAmmosTextBlock(int32 NewAmmos);
};
