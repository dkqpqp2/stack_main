// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CoinGameWidget.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API UCoinGameWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* RedTeamTextBlock;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* BlueTeamTextBlock;

public:
	void UpdateRedTeamTextBlock(int32 NewRedTeamScore);
	void UpdateBlueTeamTextBlock(int32 NewBlueTeamScore);
	
};
