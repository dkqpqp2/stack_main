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

protected:
	// UPROPERTY(VisibleAnywhere, meta=(BindWidget))

	
protected:
	// 순서 대로 초기화 됨 NativeOnInitialized -> NativePreConstruct -> NativeConstruct
	// NativeDestruct 종료 시 호출됨

	// 위젯이 생성될 때 딱 1번만 호출
	virtual void NativeOnInitialized();

	// Construct 함수들은 AddToViewport 함수를 호출하면 호출됨
	virtual void NativePreConstruct();
	virtual void NativeConstruct();


	virtual void NativeDestruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
};
