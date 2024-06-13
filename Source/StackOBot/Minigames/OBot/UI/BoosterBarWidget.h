// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BoosterBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API UBoosterBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> Image;

	UFUNCTION(BlueprintCallable)
	void SetPercent(float Percent);

};
