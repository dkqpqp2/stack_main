// Fill out your copyright notice in the Description page of Project Settings.


#include "Minigames/GameMap/CoinGame/CoinGameState.h"
#include "Net/UnrealNetwork.h"
#include "CoinGamePlayerController.h"
#include "../GameMapGameMode.h"


void ACoinGameState::OnRep_RedTeamScore()
{
	OnChangeTeamScore(true);
}

void ACoinGameState::OnRep_BlueTeamScore()
{
	OnChangeTeamScore(false);
}

void ACoinGameState::OnChangeTeamScore(bool IsRed)
{
	//Team Widget UI Update.
	ACoinGamePlayerController* CoinPC = GetWorld()->GetFirstPlayerController<ACoinGamePlayerController>();
	if (!IsValid(CoinPC))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("CoinPC Is Not Valid : ACoinGameState::OnChangeTeamScore"));
		return;
	}
	if (IsRed)
	{
		CoinPC->SetTeamScoreToWidget(IsRed, RedTeamScore);

	}
	else
	{
		CoinPC->SetTeamScoreToWidget(IsRed, BlueTeamScore);

	}
	
}

int32 ACoinGameState::GetTeamScore(bool IsRed) const
{
	return IsRed ? RedTeamScore : BlueTeamScore;
}

void ACoinGameState::SetTeamScore(bool IsRed, int32 NewScore)
{
	if (!HasAuthority())
	{
		return;
	}
	if (NewScore == 5)
	{
		// get game mode, end match, travel to lobby map again.
		Cast<AGameMode>(AuthorityGameMode)->EndMatch();
	}
	if (IsRed)
	{
		RedTeamScore = NewScore;
	}
	else
	{
		BlueTeamScore = NewScore;
	}
	OnChangeTeamScore(IsRed);
}

void ACoinGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACoinGameState, RedTeamScore);
	DOREPLIFETIME(ACoinGameState, BlueTeamScore);
}
