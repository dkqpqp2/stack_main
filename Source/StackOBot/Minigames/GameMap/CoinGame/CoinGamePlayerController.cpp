// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinGamePlayerController.h"
#include "Blueprint/UserWidget.h" 
#include "CoinGameWidget.h"


void ACoinGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		if (!IsValid(CoinGameWidgetClass))
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("CoinGameWidgetClass NOt Valid : ACoinGamePlayerController BeginPlay()"));
			return;
		}
		CoinGameWidget = CreateWidget<UCoinGameWidget>(this, CoinGameWidgetClass);
		if (!IsValid(CoinGameWidget))
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("CoinGameWidget NOt Valid : ACoinGamePlayerController BeginPlay()"));
			return;
		}
		CoinGameWidget->AddToViewport();
		CoinGameWidget->UpdateBlueTeamTextBlock(0);
		CoinGameWidget->UpdateRedTeamTextBlock(0);
	}
}

void ACoinGamePlayerController::SetTeamScoreToWidget(bool IsRedTeam, int NewTeamScore)
{
	if (!IsValid(CoinGameWidget))
	{
		// error
		return;
	}
	if (IsRedTeam)
	{
		CoinGameWidget->UpdateRedTeamTextBlock(NewTeamScore);
	}
	else
	{
		CoinGameWidget->UpdateBlueTeamTextBlock(NewTeamScore);
	}
}
