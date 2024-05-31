// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinGamePlayerState.h"
#include "CoinGamePlayerController.h"
#include "CoinGameState.h"

void ACoinGamePlayerState::OnSetScore()
{
	Super::OnSetScore();
	// widget update. 만약 플레이어 점수 위젯이 있다면 여기서 업데이트.
	ACoinGamePlayerController* PC = Cast<ACoinGamePlayerController>(GetOwningController());
	if (!IsValid(PC))
	{
		return;
	}
}
