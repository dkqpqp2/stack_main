// Fill out your copyright notice in the Description page of Project Settings.


#include "InBagAmmoWidget.h"
#include "Components/TextBlock.h"

void UInBagAmmoWidget::SetAmmosTextBlock(int32 NewAmmos)
{
	AmmosText->SetText(FText::FromString(FString::FromInt(NewAmmos)));
}
