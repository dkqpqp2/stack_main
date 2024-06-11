// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API AGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> MainWidgetClass;

	UPROPERTY()
	class UMainWidget* MainWidget;

protected:
	virtual void BeginPlay() override;
	void AddMainWidget();
};
