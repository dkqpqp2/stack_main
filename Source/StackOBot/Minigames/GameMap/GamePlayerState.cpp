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

void AGamePlayerState::OnRep_Rank()
{
	// 캐릭터 위에 띄우는 순위ui업데이트
}

void AGamePlayerState::SetRank(int32 NewRank)
{
	Rank = NewRank;
	// 캐릭터 위에 띄우는 순위ui업데이트
}

int32 AGamePlayerState::GetRank()
{
	return Rank;
}


void AGamePlayerState::OnRep_CurrentItem()
{
	// Client's Current Item Struct Set.
	if (!IsValid(ItemDataTable))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Item DataTable Not Valid"));
		return;
	}

	if (CurrentItemName != "")
	{
		CurrentItemStruct = ItemDataTable->FindRow<FItemStruct>(CurrentItemName, TEXT(""));
		UpdateItemSlotUI();
	}
	// client ui update. find item's ui by string. only if client's local player get item.
}

void AGamePlayerState::SetCurrentItem(FName NewItemName)
{
	if (HasAuthority())
	{
		if (!IsValid(ItemDataTable))
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Item DataTable Not Valid"));
			return;
		}
		CurrentItemName = NewItemName;
		CurrentItemStruct = ItemDataTable->FindRow<FItemStruct>(CurrentItemName, TEXT(""));
		
		UpdateItemSlotUI();
	}
}

void AGamePlayerState::UpdateItemSlotUI()
{
	auto PC = GetPlayerController();
	if (IsValid(PC) && PC->IsLocalController())
	{
		auto HUD = PC->GetHUD<AGameHUD>();
		if (IsValid(HUD))
		{
			HUD->MainWidget->ItemSlot->NewItemToItemSlot(CurrentItemStruct);
		}
	}
}

void AGamePlayerState::SetCurrentItemToRandomItem()
{
	if (HasAuthority())
	{
		//From Datatable RowNames, Pick One
		if (!IsValid(ItemDataTable))
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Item DataTable Not Valid"));
			return;
		}
		TArray<FName> ItemRowNames = ItemDataTable->GetRowNames();
		SetCurrentItem(ItemRowNames[FMath::RandRange(1, ItemRowNames.Num() - 1)]);
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

		if (!CurrentItemStruct)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("NoCurrent ItemStruct"));
			return;
		}

		// use CurrentItemStructure's Enum
		switch (CurrentItemStruct->ItemEnum)
		{
		case EItem::E_NONE:
			return;
			break;
		case EItem::E_BOOSTER:
			Player->OnBoosterItem();
			break;
		case EItem::E_BARRIER:
			UseBarrier();
			break;
		case EItem::E_SHIELD:
			UseShield();
			break;
		case EItem::E_MISSILE:
			// use missile item
			break;
		default:
			break;
		}

		//Empty Item...
		SetCurrentItem(FName(TEXT("NOITEM")));
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

void AGamePlayerState::UseShield()
{
	AMG_CharacterPlayer* Player = GetPawn<AMG_CharacterPlayer>();
	if (!IsValid(Player))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Pawn Not Valid When Use Shield"));
		return;
	}
	
	// 플레이어의 쉴드 함수 ON!
	Player->OnShield();
	

}


