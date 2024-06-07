// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayerState.h"
#include "../ThirdPersonCharacter.h"
#include "Net/UnrealNetwork.h"
#include "MiniGameGameState.h"
#include "../OBot/Character/MG_CharacterPlayer.h"
#include "../Item/SlowBarrier.h"

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

	DOREPLIFETIME(AGamePlayerState, CurrentItem);
}


void AGamePlayerState::OnRep_CurrentItem()
{
	// client ui update. find item's ui by string. only if client's local player get item.
}

void AGamePlayerState::SetCurrentItem(EItem NewItem)
{
	if (HasAuthority())
	{
		if (CurrentItem == EItem::E_NONE && NewItem != EItem::E_NONE)
		{
			CurrentItem = NewItem;
			// ServerUI Update, 
		}
	}

}

void AGamePlayerState::UseItem()
{
	if (HasAuthority())
	{
		APawn* Pawn = GetPawn();
		AMG_CharacterPlayer* Player = Cast<AMG_CharacterPlayer>(Pawn);
		if (!IsValid(Player))
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Pawn Not Valid When Use Item"));
			return;
		}
		switch (CurrentItem)
		{
		case EItem::E_NONE:
			break;
		case EItem::E_BOOSTER:
			Player->OnBoosterItem();
			break;
		case EItem::E_BARRIER:
			UseBarrier();
			break;
		default:
			break;
		}

		CurrentItem = EItem::E_NONE;
	}
	else
	{
		// RPC Use Item();
		SV_UseItem();
	}
}

void AGamePlayerState::UseBarrier()
{
	APawn* Pawn = GetPawn();
	AMG_CharacterPlayer* ItemUserBot = Cast<AMG_CharacterPlayer>(Pawn);
	if (!IsValid(ItemUserBot))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("ItemUserBot Not Valid : GamePS : UseBarrier()"));
		return;
	}

	// TODO : 캐릭터의 뒤편에 배리어 스폰.
	if (!IsValid(SlowBarrierBPClass))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("SlowBarrierClass Not Valid : GamePS : UseBarrier()"));
		return;
	}

	if (HasAuthority())
	{
		FVector NewLocation = ItemUserBot->GetActorLocation() - (ItemUserBot->GetActorForwardVector() * 200.f);
		GetWorld()->SpawnActor<AActor>(SlowBarrierBPClass, NewLocation, ItemUserBot->GetActorRotation());
	}
}

void AGamePlayerState::SV_UseItem_Implementation()
{
	if (HasAuthority())
	{
		UseItem();
	}
}
