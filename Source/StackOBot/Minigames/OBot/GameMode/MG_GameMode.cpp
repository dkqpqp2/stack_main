// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_GameMode.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameState.h"
#include "Minigames/OBot/Player/MG_PlayerController.h"
#include "Minigames/GameMap/GamePlayerState.h"


AMG_GameMode::AMG_GameMode()
{
	bDelayedStart = true; // 처음에 대기 상태 <날라다니기>

	static ConstructorHelpers::FClassFinder<APawn> OBotClassRef(TEXT("/Game/Character/Animation/BPMG_CharacterPlayer.BPMG_CharacterPlayer_C"));
	if (OBotClassRef.Class)
	{
		DefaultPawnClass = OBotClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<AMG_PlayerController> PlayerControllerClassRef(TEXT("/Script/StackOBot.MG_PlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
	
}

void AMG_GameMode::Tick(float Deltatime)
{
	Super::Tick(Deltatime);
	if (MatchState == MatchState::WaitingToStart)
	{
		CountDownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountDownTime <= 0.f)
		{
			StartMatch();
		}
	}
}

void AMG_GameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AMG_PlayerController* MGPlayer = Cast<AMG_PlayerController>(*It);
		if (MGPlayer)
		{
			MGPlayer->OnMatchStateSet(MatchState);
		}
	}
}

void AMG_GameMode::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();

	// 모든 캐릭터 플레이어를 가져와서
	//GetGameState<AGameState>()->PlayerArray[0]->GetPawn();
	
	// 각각 캐릭터의 (시상식으로 (등수에 맞게) 이동시키고 카메라를 전환시키고, 입력비활성화하는)함수를 호출

	// 몇초 뒤에 시상식 종료... 로비화면으로 이동?
}


void AMG_GameMode::BeginPlay()
{
	Super::BeginPlay();

	LevelStartingTime = GetWorld()->GetTimeSeconds();
}

