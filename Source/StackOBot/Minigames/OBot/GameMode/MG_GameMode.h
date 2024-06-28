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
	extern STACKOBOT_API const FName CoolDown; // begin coolTimer
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
	float WarmupTime = 5.f;

	UPROPERTY(EditDefaultsOnly)
	float CoolDownAnnounceTime = 10.f;

	UFUNCTION()
	void OnPlayerFinishLineReached();

	float LevelStartingTime = 0.f;
	bool bCoolDown = false; // 플래그
	float ServerTimeAtCoolDown = 0.f;


protected:
	virtual void BeginPlay() override;
	virtual void OnMatchStateSet() override;
	virtual void HandleMatchHasEnded() override;

	// 모든 플레이어들의 결승선까지의 거리를 재고 그에 따라 각 플레이어의 순위 결정.
	// 각 플레이어들의 UI업데이트는?
	void UpdatePlayersRank();
	FTimerHandle UpdatePlayersRankTimer;
	AActor* FinishActor;
private:
	float CountDownTime = 0.f;

protected:
	FTimerHandle FinishLevelTimer;
	void ReturnToLobby();
};
