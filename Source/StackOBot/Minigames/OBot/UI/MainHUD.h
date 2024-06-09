// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUD.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API UMainHUD : public UUserWidget
{
	GENERATED_BODY()
public:
    virtual void NativeConstruct() override;

    void UpdateHoveringProgress(float Percent);
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UBoosterBarWidget> BoosterBar;
};
