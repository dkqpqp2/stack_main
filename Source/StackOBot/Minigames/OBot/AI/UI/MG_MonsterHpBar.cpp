// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_MonsterHpBar.h"
#include "Components/ProgressBar.h"
#include "Minigames/OBot/Interface/MG_CharacterWidgetInterface.h"

UMG_MonsterHpBar::UMG_MonsterHpBar(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void UMG_MonsterHpBar::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	ensure(HpProgressBar);

	IMG_CharacterWidgetInterface* CharacterWidget = Cast<IMG_CharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}

}

void UMG_MonsterHpBar::UpdateHpBar(float NewCurrentHp)
{
	ensure(MaxHp > 0.0f);
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
	}
}
