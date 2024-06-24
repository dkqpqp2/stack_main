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
	AMG_ShootingCharacterPlayer* PlayerOwner = Cast<AMG_ShootingCharacterPlayer>(GetPawn());
	PlayerOwner = PlayerOwner == nullptr ? Cast<AMG_ShootingCharacterPlayer>(GetPawn()) : PlayerOwner;
	if (PlayerOwner)
		PlayerOwner->UpdateHUDHealth();
}

void AFPSPlayerController::SetHUDHealth(float CurrentHealth, float MaxHealth, FString PlayerName)
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

void AFPSPlayerController::SetHUDHealth_1(float CurrentHealth, float MaxHealth, FString PlayerName)
{
	FPSHUD = FPSHUD == nullptr ? Cast<AFPSHUD>(GetHUD()) : FPSHUD;

	bool bValid = FPSHUD &&
		FPSHUD->FPSOverlay &&
		FPSHUD->FPSOverlay->HealthBar_1 &&
		FPSHUD->FPSOverlay->Player1;

	if (bValid)
	{
		const float HealthPercent = CurrentHealth / MaxHealth;
		FPSHUD->FPSOverlay->HealthBar_1->SetPercent(HealthPercent);
		FPSHUD->FPSOverlay->Player1->SetText(FText::FromString(PlayerName));
		//텍스트 블락에 ps에 가져온 이름을 배치하는 함수 
		//FPSHUD->FPSOverlay->Player1
	}
}


void AFPSPlayerController::SetHUDHealth_2(float CurrentHealth, float MaxHealth, FString PlayerName)
{
	FPSHUD = FPSHUD == nullptr ? Cast<AFPSHUD>(GetHUD()) : FPSHUD;

	bool bValid = FPSHUD &&
		FPSHUD->FPSOverlay &&
		FPSHUD->FPSOverlay->HealthBar_2;

	if (bValid)
	{
		const float HealthPercent = CurrentHealth / MaxHealth;
		FPSHUD->FPSOverlay->HealthBar_2->SetPercent(HealthPercent);
		FPSHUD->FPSOverlay->Player2->SetText(FText::FromString(PlayerName));
	}
}


void AFPSPlayerController::SetHUDHealth_3(float CurrentHealth, float MaxHealth, FString PlayerName)
{
	FPSHUD = FPSHUD == nullptr ? Cast<AFPSHUD>(GetHUD()) : FPSHUD;

	bool bValid = FPSHUD &&
		FPSHUD->FPSOverlay &&
		FPSHUD->FPSOverlay->HealthBar_3;

	if (bValid)
	{
		const float HealthPercent = CurrentHealth / MaxHealth;
		FPSHUD->FPSOverlay->HealthBar_3->SetPercent(HealthPercent);
		FPSHUD->FPSOverlay->Player3->SetText(FText::FromString(PlayerName));
	}
}

void AFPSPlayerController::SetHUDAmmo(int32 Ammo, int32 MaxAmmo)
{
	FPSHUD = FPSHUD == nullptr ? Cast<AFPSHUD>(GetHUD()) : FPSHUD;

	bool bValid = FPSHUD &&
		FPSHUD->FPSOverlay &&
		FPSHUD->FPSOverlay->CurrentBullet &&
		FPSHUD->FPSOverlay->MaxBullet;

	if (bValid)
	{
		FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
		FString MaxAmmoText = FString::Printf(TEXT("%d"), MaxAmmo);
		FPSHUD->FPSOverlay->CurrentBullet->SetText(FText::FromString(AmmoText));
		FPSHUD->FPSOverlay->MaxBullet->SetText(FText::FromString(MaxAmmoText));
	}
}


