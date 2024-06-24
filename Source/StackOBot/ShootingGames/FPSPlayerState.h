// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "../Minigames/GameMap/GamePlayerState.h"
#include "FPSPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API AFPSPlayerState : public AGamePlayerState
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
};
