// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Minigames/Item/ItemBase.h"
#include "ItemSlowBarrier.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API UItemSlowBarrier : public UItemBase
{
	GENERATED_BODY()
	UItemSlowBarrier();

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	TSubclassOf<AActor> SpawnToActor;

public:
	virtual void ActivateItem(APawn* const ItemUser) override;
	
};
