// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSPlayerController.h"
#include "FPSHUD.h"
#include "FPSOverlay.h"
#include "GameFramework/Pawn.h"
#include "../Source/StackOBot/MiniGames/Obot/Character/MG_ShootingCharacterPlayer.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void AFPSPlayerController::BeginPlay()
{
	Super::BeginPlay();
	FPSHUD = Cast<AFPSHUD>(GetHUD());
}

void AFPSPlayerController::SetHUDHealth(float CurrentHealth, float MaxHealth)
{
	FPSHUD = FPSHUD == nullptr ? Cast<AFPSHUD>(GetHUD()) : FPSHUD;

	bool bValid = FPSHUD && 
		FPSHUD->FPSOverlay && 
		FPSHUD->FPSOverlay->HealthBar && 
		FPSHUD->FPSOverlay->HealthText;

	if (bValid)
	{
		const float HealthPercent = CurrentHealth / MaxHealth;
		FPSHUD->FPSOverlay->HealthBar->SetPercent(HealthPercent);
		//FPSHUD->FPSOverlay->HealthBar_1->SetPercent();
		//FPSHUD->FPSOverlay->HealthBar_2->SetPercent();
		//FPSHUD->FPSOverlay->HealthBar_3->SetPercent();
		FString HealthtText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(CurrentHealth), FMath::CeilToInt(MaxHealth));
		FPSHUD->FPSOverlay->HealthText->SetText(FText::FromString(HealthtText));
	}
}

void AFPSPlayerController::SetHUDHealth_1(float CurrentHealth, float MaxHealth)
{
	FPSHUD = FPSHUD == nullptr ? Cast<AFPSHUD>(GetHUD()) : FPSHUD;

	bool bValid = FPSHUD &&
		FPSHUD->FPSOverlay &&
		FPSHUD->FPSOverlay->HealthBar_1;

	if (bValid)
	{
		const float HealthPercent = CurrentHealth / MaxHealth;
		FPSHUD->FPSOverlay->HealthBar_1->SetPercent(HealthPercent);
	}
}


void AFPSPlayerController::SetHUDHealth_2(float CurrentHealth, float MaxHealth)
{
	FPSHUD = FPSHUD == nullptr ? Cast<AFPSHUD>(GetHUD()) : FPSHUD;

	bool bValid = FPSHUD &&
		FPSHUD->FPSOverlay &&
		FPSHUD->FPSOverlay->HealthBar_2;

	if (bValid)
	{
		const float HealthPercent = CurrentHealth / MaxHealth;
		FPSHUD->FPSOverlay->HealthBar_2->SetPercent(HealthPercent);
	}
}


void AFPSPlayerController::SetHUDHealth_3(float CurrentHealth, float MaxHealth)
{
	FPSHUD = FPSHUD == nullptr ? Cast<AFPSHUD>(GetHUD()) : FPSHUD;

	bool bValid = FPSHUD &&
		FPSHUD->FPSOverlay &&
		FPSHUD->FPSOverlay->HealthBar_3;

	if (bValid)
	{
		const float HealthPercent = CurrentHealth / MaxHealth;
		FPSHUD->FPSOverlay->HealthBar_3->SetPercent(HealthPercent);
	}
}


