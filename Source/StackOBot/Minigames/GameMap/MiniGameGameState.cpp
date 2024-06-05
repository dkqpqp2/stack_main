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
		
		AvailableItems.Add(Cast<UItemBase>(ItemClass->GetDefaultObject(true)));
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

		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, TEXT("GetItem()"));

		// 우선은 0번아이템 전달.
		return AvailableItems[0];
	}
	return nullptr;
}
