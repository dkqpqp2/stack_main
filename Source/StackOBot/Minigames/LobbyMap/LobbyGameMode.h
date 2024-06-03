// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

UCLASS(minimalapi)
class ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALobbyGameMode();

public:
	// 미니게임이 끝나고 로비로 돌아왔을 때, 각 플레이어 스폰, 빙의 처리.
	virtual void RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot) override;
	virtual void PostSeamlessTravel() override;

	// 시작화면에서 로비로 진입할때 각 플레이어의 세팅.
	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	int32 PlayerIDOfNextPlayer = 0;

public:
	void ServerTravelToGameMap();
};



