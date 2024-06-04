// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Item/ItemBase.h"
#include "GameFramework/GameState.h"
#include "MiniGameGameState.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API AMiniGameGameState : public AGameState
{
	GENERATED_BODY()
public:
	AMiniGameGameState();

protected:
	virtual void BeginPlay() override;
	
	// --------- Item List --------
protected:
	UPROPERTY(EditAnywhere);
	TArray< TSubclassOf<UItemBase>> ItemClasses;
	TArray<UItemBase*> AvailableItems;

public:
	UItemBase* GetItem();
};
