// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MG_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API AMG_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void SetHUDMatchCountdown(float CountdownTime);
	void SetHUDTime();

private:
    class AGameHUD* GameHUD;

	float MatchTime = 10.0f;
	uint32 CountDown = 0;
};
