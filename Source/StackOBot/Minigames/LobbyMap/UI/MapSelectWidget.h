// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapSelectWidget.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API UMapSelectWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	class UButton* BeforeButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* NextButton;	
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MapTextBlock;

public:
	void UpdateCurrentMap();
	void EnableButton();
protected:
	UFUNCTION()
	void OnClickBeforeButton();

	UFUNCTION()
	void OnClickNextButton();
};
