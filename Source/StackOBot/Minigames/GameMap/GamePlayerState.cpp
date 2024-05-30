// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayerState.h"
#include "../ThirdPersonCharacter.h"

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
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, TEXT("GamePlayerState's IsRedTeam Setted"));
	IsRedTeam = IsChecked;
}
void AGamePlayerState::SetSelectedCharacter(FString NewCharacter)
{
	SelectedCharacter = NewCharacter;
}

void AGamePlayerState::SetPlayerPawn(APlayerState* Player, APawn* NewPawn, APawn* OldPawn)
{
	if (!HasAuthority())
	{
		ITeamChangeInterface* GamePawn = Cast<ITeamChangeInterface>(NewPawn);
		if (GamePawn == nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Pawn Does Not Implemets ITeamChangeInterface"));
			return;
		}
		GamePawn->SetMaterialToTeamColor(IsRedTeam);

	}

}
