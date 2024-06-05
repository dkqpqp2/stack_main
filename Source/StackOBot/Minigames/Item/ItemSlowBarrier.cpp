// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSlowBarrier.h"
#include "../OBot/Character/MG_CharacterPlayer.h"

UItemSlowBarrier::UItemSlowBarrier()
{
	ItemName = TEXT("SlowBarrier");
}

void UItemSlowBarrier::ActivateItem(APawn* const ItemUser)
{
	AMG_CharacterPlayer* ItemUserBot = Cast<AMG_CharacterPlayer>(ItemUser);
	if (!IsValid(ItemUserBot))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("ItemUserBot Not Valid : UItemSlowBarrier::ActivateItem()"));
		return;
	}

	// TODO : 캐릭터의 뒤편에 배리어 스폰.
	if (IsValid(SpawnToActor))
	{
		
		GetWorld()->SpawnActor<AActor>(SpawnToActor, ItemUser->GetActorLocation(), ItemUser->GetActorRotation());

	}
}
