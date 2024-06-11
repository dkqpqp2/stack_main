// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../../GameMap/GamePlayerController.h"
#include "Net/UnrealNetwork.h"
#include "MG_PlayerController.generated.h"
/**
 * 
 */
UCLASS()
class STACKOBOT_API AMG_PlayerController : public AGamePlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SetHUDMatchCountdown(float CountdownTime);
	void SetHUDTime();

	void SetHUDMatchCountUp();

private:
    class AGameHUD* GameHUD;

	float MatchTime = 10.0f; // 골인 지점 도착 후 시간 측정 
	uint32 CountDown = 0;
};
