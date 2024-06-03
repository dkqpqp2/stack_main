// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Minigames/GameMap/GamePlayerState.h"
#include "CoinGamePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API ACoinGamePlayerState : public AGamePlayerState
{
	GENERATED_BODY()
	
	// PlayerState 이미 float Score가 있다.
	// OnRep도 있고.

public:
	/** Replication Notification Callbacks */
	virtual void OnSetScore() override;
};
