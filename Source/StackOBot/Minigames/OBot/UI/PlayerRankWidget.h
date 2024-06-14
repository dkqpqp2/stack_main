// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerRankWidget.generated.h"


class UTextBlock;
/**
 * 
 */
UCLASS()
class STACKOBOT_API UPlayerRankWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> PlayerRankTextBlock;

public:
	void SetPlayerRankTextBlock(int32 NewRank);
	
};
