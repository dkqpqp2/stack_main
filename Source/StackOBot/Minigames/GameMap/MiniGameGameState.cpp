// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniGameGameState.h"
#include "../Item/ItemClassDataTableRow.h"

AMiniGameGameState::AMiniGameGameState()
{
	//static ConstructorHelpers::FObjectFinder<UDataTable> DT_ItemClasses(
	//	TEXT("")
	//);
	//if (DT_ItemClasses.Succeeded())
	//{
	//	DT_ItemClasses.Object;
	//}
}

void AMiniGameGameState::BeginPlay()
{
	Super::BeginPlay();

	for (auto ItemClass : ItemClasses)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("GameState NewObject"));
		
		AvailableItems.Add(NewObject<UItemBase>(this, ItemClass));
		//AvailableItems.Add(Cast<UItemBase>(ItemClass->GetDefaultObject(true)));
	}

}

UItemBase* AMiniGameGameState::GetItem()
{
	if (HasAuthority())
	{
		if (AvailableItems.IsEmpty())
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("No AvailableItem In GameState"));
			return nullptr;
		}

		// 랜덤으로 아이템 전달.
		int32 RandomIndex = FMath::RandRange(0, AvailableItems.Num() - 1);
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, TEXT("GetItem()"));

		return AvailableItems[RandomIndex];
	}
	return nullptr;
}
