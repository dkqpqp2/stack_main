// Fill out your copyright notice in the Description page of Project Settings.


#include "Minigames/OBot/PlayerState/MG_PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Minigames/GameMap/MiniGameGameState.h"
#include "Minigames/OBot/Character/MG_CharacterPlayer.h"
#include "Minigames/Item/SlowBarrier.h"
#include "Minigames/GameMap/GameHUD.h"
#include "MainWidget.h"
#include "Minigames/GameMap/UI/ItemSlotWidget.h"
#include "Minigames/OBot/Projectile/MG_MissileTest.h"

void AMG_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMG_PlayerState, CurrentItemName);
	DOREPLIFETIME(AMG_PlayerState, Rank);

}

void AMG_PlayerState::OnRep_Rank()
{
	UpdateRankUI();
}

void AMG_PlayerState::UpdateRankUI()
{
	// 캐릭터 위에 띄우는 순위ui업데이트
	AMG_CharacterPlayer* Character = GetPawn<AMG_CharacterPlayer>();
	if (IsValid(Character))
	{
		Character->SetPlayerRankWidget(Rank);
	}
}

void AMG_PlayerState::SetRank(int32 NewRank)
{
	if (Rank == NewRank)
	{
		return;
	}
	Rank = NewRank;
	UpdateRankUI();
}

int32 AMG_PlayerState::GetRank()
{
	return Rank;
}


void AMG_PlayerState::OnRep_CurrentItem()
{
	// Client's Current Item Struct Set.
	if (!IsValid(ItemDataTable))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Item DataTable Not Valid"));
		return;
	}

	if (CurrentItemName != "")
	{
		CurrentItemStruct = ItemDataTable->FindRow<FItemStruct>(CurrentItemName, TEXT(""));
		UpdateItemSlotUI();
	}
	// client ui update. find item's ui by string. only if client's local player get item.
}

void AMG_PlayerState::SetCurrentItem(FName NewItemName)
{
	if (HasAuthority())
	{
		if (!IsValid(ItemDataTable))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Item DataTable Not Valid"));
			return;
		}
		CurrentItemName = NewItemName;
		CurrentItemStruct = ItemDataTable->FindRow<FItemStruct>(CurrentItemName, TEXT(""));

		UpdateItemSlotUI();
	}
}

void AMG_PlayerState::UpdateItemSlotUI()
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

void AMG_PlayerState::SetCurrentItemToRandomItem()
{
	if (HasAuthority())
	{
		//From Datatable RowNames, Pick One
		if (!IsValid(ItemDataTable))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Item DataTable Not Valid"));
			return;
		}
		TArray<FName> ItemRowNames = ItemDataTable->GetRowNames();
		SetCurrentItem(ItemRowNames[FMath::RandRange(1, ItemRowNames.Num() - 1)]);
	}

}

void AMG_PlayerState::UseItem()
{
	if (HasAuthority())
	{
		APawn* Pawn = GetPawn();
		AMG_CharacterPlayer* Player = Cast<AMG_CharacterPlayer>(Pawn);
		if (!IsValid(Player))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Pawn Not Valid When Use Item"));
			return;
		}

		if (!CurrentItemStruct)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("NoCurrent ItemStruct"));
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
			UseMissile();
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

void AMG_PlayerState::SV_UseItem_Implementation()
{
	if (HasAuthority())
	{
		UseItem();
	}
}

void AMG_PlayerState::UseBarrier()
{
	APawn* Pawn = GetPawn();
	AMG_CharacterPlayer* ItemUserBot = Cast<AMG_CharacterPlayer>(Pawn);
	if (!IsValid(ItemUserBot))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("ItemUserBot Not Valid : GamePS : UseBarrier()"));
		return;
	}

	// TODO : 캐릭터의 뒤편에 배리어 스폰.
	if (!IsValid(SlowBarrierBPClass))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("SlowBarrierClass Not Valid : GamePS : UseBarrier()"));
		return;
	}

	if (HasAuthority())
	{
		FVector NewLocation = ItemUserBot->GetActorLocation() - (ItemUserBot->GetActorForwardVector() * 200.f);
		GetWorld()->SpawnActor<AActor>(SlowBarrierBPClass, NewLocation, ItemUserBot->GetActorRotation());
	}
}

void AMG_PlayerState::UseShield()
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

void AMG_PlayerState::UseMissile()
{
	if (Rank == 1)
	{
		return;
	}
	int32 FindRank = Rank - 1;
	AActor* Actor = GetWorld()->SpawnActor(AMG_MissileTest::StaticClass(), &GetPawn()->GetActorTransform());
	AMG_MissileTest* Missile = Cast< AMG_MissileTest>(Actor);

	auto Players = GetWorld()->GetGameState()->PlayerArray;

	for (auto Player : Players)
	{
		AMG_PlayerState* PS = Cast<AMG_PlayerState>(Player);
		if (IsValid(PS))
		{
			if (PS->GetRank() == FindRank)
			{
				Missile->FindPlayer(PS->GetPawn<AMG_CharacterBase>());

				break;
			}
		}

	}

}