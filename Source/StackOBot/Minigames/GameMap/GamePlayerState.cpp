// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayerState.h"
#include "../ThirdPersonCharacter.h"
#include "../Item/ItemBase.h"
#include "Net/UnrealNetwork.h"
#include "MiniGameGameState.h"

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
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Pawn Does Not Implemets ITeamChangeInterface"));
			return;
		}
		GamePawn->SetMaterialToTeamColor(IsRedTeam);

	}

}

void AGamePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGamePlayerState, CurrentItemName);
}


void AGamePlayerState::OnRep_CurrentItemName()
{
	// client ui update. find item's ui by string. only if client's local player get item.
}

void AGamePlayerState::SetCurrentItem(UItemBase* NewItem)
{
	if (HasAuthority())
	{
		if (CurrentItem == nullptr)
		{
			if (IsValid(NewItem))
			{
				CurrentItem = NewItem;
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, TEXT("Item Setted"));

				// ItemData Update.
				CurrentItemName = NewItem->GetItemName();

				// Server UI Update only if server's local player get item. 

			}

		}
	}

}

void AGamePlayerState::UseItem()
{
	if (HasAuthority())
	{
		APawn* Pawn = GetPawn();
		if (!IsValid(Pawn))
		{
			return;
		}
		if (!IsValid(CurrentItem))
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("No Item"));
			return;
		}
		CurrentItem->ActivateItem(Pawn);
	}
	else
	{
		// RPC Use Item();
		SV_UseItem();
	}
}

void AGamePlayerState::SV_UseItem_Implementation()
{
	if (HasAuthority())
	{
		UseItem();
	}
}
