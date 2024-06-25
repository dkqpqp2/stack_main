// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "MG_WidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API UMG_WidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()


protected:
	virtual void InitWidget() override;
	
};
