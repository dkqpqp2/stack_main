// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinGameWidget.h"
#include "Components/TextBlock.h"

void UCoinGameWidget::UpdateRedTeamTextBlock(int32 NewRedTeamScore)
{
	RedTeamTextBlock->SetText(FText::FromString(FString::Printf(TEXT("Team Red Score : %d"), NewRedTeamScore)));
}

void UCoinGameWidget::UpdateBlueTeamTextBlock(int32 NewBlueTeamScore)
{
	BlueTeamTextBlock->SetText(FText::FromString(FString::Printf(TEXT("Team Blue Score : %d"), NewBlueTeamScore)));

}
