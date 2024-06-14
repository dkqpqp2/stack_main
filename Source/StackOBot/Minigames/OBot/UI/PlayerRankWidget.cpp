// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerRankWidget.h"
#include "Components/TextBlock.h"

void UPlayerRankWidget::SetPlayerRankTextBlock(int32 NewRank)
{
	PlayerRankTextBlock->SetText(FText::FromString( FString::FromInt(NewRank)));
}
