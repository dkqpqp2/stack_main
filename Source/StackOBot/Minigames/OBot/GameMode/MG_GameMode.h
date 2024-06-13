// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameMode.h"
#include "Minigames/OBot/Character/MG_CharacterPlayer.h"
#include "../../GameMap/GameMapGameMode.h"
#include "MG_GameMode.generated.h"

namespace MatchState
{
	extern UMG_API const FName CoolDown; // begin coolTimer
}
/**
 * 
 */
UCLASS()
class STACKOBOT_API AMG_GameMode : public AGameMapGameMode
{
	GENERATED_BODY()

public:
	AMG_GameMode();
	virtual void Tick(float Deltatime) override; // 시간tick별 재야 하니까

	UPROPERTY(EditDefaultsOnly)
	float WarmupTime = 1.f;
	float LevelStartingTime = 0.f;

	UPROPERTY(EditDefaultsOnly)
	float CoolDownTime = 10.f;

	UFUNCTION()
	void OnPlayerFinishLineReached();


protected:
	virtual void BeginPlay() override;
	virtual void OnMatchStateSet() override;
	virtual void HandleMatchHasEnded() override;

private:
	float CountDownTime = 0.f;
};
