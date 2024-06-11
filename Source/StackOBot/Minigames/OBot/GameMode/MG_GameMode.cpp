// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_GameMode.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/GameModeBase.h"
#include "Minigames/OBot/Player/MG_PlayerController.h"
#include "Minigames/GameMap/GamePlayerState.h"


AMG_GameMode::AMG_GameMode()
{
	AGameModeBase::bStartPlayersAsSpectators = true;
	bDelayedStart = true;

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

void AMG_GameMode::BeginPlay()
{
	Super::BeginPlay();

	LevelStartingTime = GetWorld()->GetTimeSeconds();
}
