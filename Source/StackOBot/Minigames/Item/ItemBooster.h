// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Minigames/Item/ItemBase.h"
#include "ItemBooster.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API UItemBooster : public UItemBase
{
	GENERATED_BODY()
	UItemBooster();
public:
	virtual void ActivateItem(APawn* const ItemUser);
};
