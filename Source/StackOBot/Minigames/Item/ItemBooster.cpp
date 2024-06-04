// Fill out your copyright notice in the Description page of Project Settings.


#include "Minigames/Item/ItemBooster.h"
#include "../OBot/Character/MG_CharacterPlayer.h"


void AItemBooster::ActivateItem(APawn* const ItemUser)
{
	AMG_CharacterPlayer* ItemUserBot = Cast<AMG_CharacterPlayer>(ItemUser);
	if (!IsValid(ItemUserBot))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("ItemUserBot Not Valid : UItemBooster::ActivateItem()"));
		return;
	}

	// TODO : 캐릭터의 부스트 함수 호출.(OnUseBooserItem)
	ItemUserBot->OnBoosterItem();

}
