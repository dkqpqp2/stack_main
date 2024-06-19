// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayerState.h"
#include "../ThirdPersonCharacter.h"
#include "Net/UnrealNetwork.h"
#include "MiniGameGameState.h"
#include "../OBot/Character/MG_CharacterPlayer.h"
#include "../Item/SlowBarrier.h"
#include "GameHUD.h"
#include "../../MainWidget.h"
#include "UI/ItemSlotWidget.h"
#include "Minigames/OBot/Projectile/MG_MissileTest.h"

void AGamePlayerState::BeginPlay()
{
	Super::BeginPlay();
	OnPawnSet.AddDynamic(this, &ThisClass::SetPlayerPawn);
}

void AGamePlayerState::SetPlayerEnterID(int32 NewEnterID)
{
	PlayerEnterID = NewEnterID;
}

void AGamePlayerState::SetIsRedTeamTo(bool IsChecked)
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, TEXT("GamePlayerState's IsRedTeam Setted"));
	IsRedTeam = IsChecked;
}
void AGamePlayerState::SetSelectedCharacter(FString NewCharacter)
{
	SelectedCharacter = NewCharacter;
}

void AGamePlayerState::TrySetScore(float NewScore)
{
	if (HasAuthority())
	{
		SetScore(NewScore);
		// On SetScore() widget update...
		OnSetScore();
	}
	else
	{
		// RPC To Server.
		SV_TrySetScore(NewScore);
	}
}

void AGamePlayerState::OnSetScore()
{
	// widget update.
}

void AGamePlayerState::OnRep_Score()
{
	OnSetScore();
}

void AGamePlayerState::SV_TrySetScore_Implementation(float NewScore)
{
	SetScore(NewScore);
	// OnSetScore(); widget update...
	OnSetScore();
}

void AGamePlayerState::SetPlayerPawn(APlayerState* Player, APawn* NewPawn, APawn* OldPawn)
{
	if (!HasAuthority())
	{
		ITeamChangeInterface* GamePawn = Cast<ITeamChangeInterface>(NewPawn);
		if (GamePawn == nullptr)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Pawn Does Not Implemets ITeamChangeInterface"));
			return;
		}
		GamePawn->SetMaterialToTeamColor(IsRedTeam);

	}

}



