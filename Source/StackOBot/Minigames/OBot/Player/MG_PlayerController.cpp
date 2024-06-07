// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_PlayerController.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Minigames/GameMap/GameHud.h"
#include "../Source/StackOBot/MainWidget.h"	

void AMG_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	//GameHUD = Cast<AGameHUD>(GetHUD());
}

void AMG_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetHUDTime();
}

void AMG_PlayerController::SetHUDMatchCountdown(float CountdownTime)
{
	GameHUD = GameHUD == nullptr ? Cast<AGameHUD>(GetHUD()) : GameHUD;
	bool bHUDValid = GameHUD &&
		GameHUD->MainWidget &&
		GameHUD->MainWidget->MatchCountdownText;
	if (bHUDValid)
	{
		int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
		int32 Seconds = CountdownTime - Minutes * 60;

		FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		GameHUD->MainWidget->MatchCountdownText->SetText(FText::FromString(CountdownText));
	}
}

void AMG_PlayerController::SetHUDTime()
{
	uint32 SecondsLeft = FMath::CeilToInt(MatchTime - GetWorld()->GetTimeSeconds());
	if (CountDown != SecondsLeft)
	{
		SetHUDMatchCountdown(MatchTime - GetWorld()->GetTimeSeconds());
	}

	CountDown = SecondsLeft;
}

/*void AMG_PlayerController::SetHUDHover(float Hover, float MaxHover)
{
	GameHUD = GameHUD == nullptr ? Cast<AGameHUD>(GetHUD()) : GameHUD;
	bool bHUDValid = GameHUD && 
		GameHUD -> 
}
*/