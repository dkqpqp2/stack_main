// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../MiniGameGameState.h"
#include "GameFramework/GameState.h"
#include "CoinGameState.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API ACoinGameState : public AMiniGameGameState
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(ReplicatedUsing = OnRep_RedTeamScore)
	int32 RedTeamScore = 0;

	UPROPERTY(ReplicatedUsing = OnRep_BlueTeamScore)
	int32 BlueTeamScore = 0;

	UFUNCTION()
	void OnRep_RedTeamScore();

	UFUNCTION()
	void OnRep_BlueTeamScore();

protected:
	void OnChangeTeamScore(bool IsRed);
public:
	int32 GetTeamScore(bool IsRed) const;
	void SetTeamScore(bool IsRed, int32 NewScore);

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
