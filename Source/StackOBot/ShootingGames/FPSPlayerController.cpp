// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSPlayerController.h"
#include "FPSHUD.h"
#include "FPSOverlay.h"
#include "GameFramework/Pawn.h"
#include "../Source/StackOBot/MiniGames/Obot/Character/MG_ShootingCharacterPlayer.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Curves/CurveLinearColor.h"
#include "../ShootingGames/GamMode/ShootingGameMode.h"
#include "Weapons/InBagAmmoWidget.h"

void AFPSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = false;
	FInputModeGameOnly GameInputMode;
	SetInputMode(GameInputMode);

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
		FLinearColor HPColor = GetHealthColor(HealthPercent);
		FPSHUD->FPSOverlay->HealthBar->SetFillColorAndOpacity(HPColor);
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
		FLinearColor HPColor = GetHealthColor(HealthPercent);
		FPSHUD->FPSOverlay->HealthBar_1->SetFillColorAndOpacity(HPColor);
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
		FLinearColor HPColor = GetHealthColor(HealthPercent);
		FPSHUD->FPSOverlay->HealthBar_2->SetFillColorAndOpacity(HPColor);
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
		FLinearColor HPColor = GetHealthColor(HealthPercent);
		FPSHUD->FPSOverlay->HealthBar_3->SetFillColorAndOpacity(HPColor);
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

void AFPSPlayerController::SetHUDInHandAmmo(int32 NewAmmo, EWeaponType AmmoType)
{
	FPSHUD = FPSHUD == nullptr ? Cast<AFPSHUD>(GetHUD()) : FPSHUD;

	bool bValid = FPSHUD &&
		FPSHUD->FPSOverlay;

	if (bValid)
	{
		switch (AmmoType)
		{
		case EWeaponType::Rifle:
			FPSHUD->FPSOverlay->RifleInHandAmmos->SetAmmosTextBlock(NewAmmo);
			break;
		case EWeaponType::Pistol:
			FPSHUD->FPSOverlay->PistolInHandAmmos->SetAmmosTextBlock(NewAmmo);
			break;
		case EWeaponType::Grenade:
			FPSHUD->FPSOverlay->GrenadeInHandAmmos->SetAmmosTextBlock(NewAmmo);
			break;
		default:
			break;
		}
	}
}

void AFPSPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetHUDTime(); //매초 시간 hud 변경
	CheckTimeSync(DeltaTime);
}

float AFPSPlayerController::GetServerTime()
{
	if (HasAuthority()) return GetWorld()->GetTimeSeconds();
	else return GetWorld()->GetTimeSeconds() + ClientServerDelta;
}

void AFPSPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
	if (IsLocalController())
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
	}
}


FLinearColor AFPSPlayerController::GetHealthColor(float HP)
{
	HealthColor = HealthCurve->GetLinearColorValue(HP);
	return HealthColor;
}

void AFPSPlayerController::SetHUDMatchCountDown(float CountDownTime)
{
	FPSHUD = FPSHUD == nullptr ? Cast<AFPSHUD>(GetHUD()) : FPSHUD;
	bool bHUDValid = FPSHUD &&
		FPSHUD->FPSOverlay &&
		FPSHUD->FPSOverlay->MatchCountDownText;
	if (bHUDValid)
	{
		int32 Minutes = FMath::FloorToInt(CountDownTime / 60.f);
		int32 Seconds = CountDownTime - Minutes * 60;

		FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		FPSHUD->FPSOverlay->MatchCountDownText->SetText(FText::FromString(CountdownText));
	}
}

void AFPSPlayerController::SetHUDTime()
{
	uint32 SecondsLeft = FMath::CeilToInt(MatchTime - GetWorld()->GetTimeSeconds());
	if (CountDownInt != SecondsLeft)
	{
		SetHUDMatchCountDown(MatchTime - GetServerTime());
	}

	CountDownInt = SecondsLeft;
}

void AFPSPlayerController::ClientReportServerTime_Implementation(float TimeOfClientRequest, float TimeServerReceivedClientRequest)
{
	float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest;
	float CurrentServerTime = TimeServerReceivedClientRequest + (0.5 * RoundTripTime);
	ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds();
}

void AFPSPlayerController::ServerRequestServerTime_Implementation(float TimeOfClientRequest)
{
	float ServerTimeOfReceipt = GetWorld()->GetTimeSeconds();
	ClientReportServerTime(TimeOfClientRequest, ServerTimeOfReceipt);
}

void AFPSPlayerController::CheckTimeSync(float DeltaTime)
{
	TimeSyncRunningTime += DeltaTime;
	if (IsLocalController() && TimeSyncRunningTime > TimeSyncFrequency)
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
		TimeSyncRunningTime = 0.f;
	}
}

void AFPSPlayerController::OnMatchStateSet(FName State)
{
	MatchState = State;
	if (MatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted();
	}
	if (MatchState == MatchState::WaitingPostMatch)
	{
		HandleMatchHasEnded();
	}
}

void AFPSPlayerController::OnRep_MatchState()
{
	if (MatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted();
	}
	if (MatchState == MatchState::WaitingPostMatch)
	{
		HandleMatchHasEnded();
	}
}

void AFPSPlayerController::HandleMatchHasStarted()
{
	//영상 보여주는거 넣으면 될듯 
	//
}

void AFPSPlayerController::HandleMatchHasEnded()
{
	//UI띄우고 Announce <ex> 게임이 종료되었습니다 
	if (bIsWinner)
	{
		UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);
		if (WinScreen != nullptr)
		{
			WinScreen->AddToViewport();
		}
	}
	else
	{
		UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass );
		if (LoseScreen != nullptr)
		{
			LoseScreen->AddToViewport();
		}
	}
}

void AFPSPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFPSPlayerController, MatchState)
	DOREPLIFETIME(AFPSPlayerController, bIsWinner)
}
