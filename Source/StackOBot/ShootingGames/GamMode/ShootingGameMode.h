// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Minigames/OBot/GameMode/MG_GameMode.h"
#include "ShootingGameMode.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API AShootingGameMode : public AGameMapGameMode
{
	GENERATED_BODY()
public:

	UFUNCTION()
	void OnPlayerFinishLineReached();

	void AllPlayerDeadCheck();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated)
	bool IsWin;
protected:

	virtual void BeginPlay() override;
	virtual void OnMatchStateSet() override;
	virtual void HandleMatchHasEnded() override;

	
};
