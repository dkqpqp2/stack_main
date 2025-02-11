// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_PlayerController.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Minigames/GameMap/GameHud.h"
#include "../Source/StackOBot/MainWidget.h"	
#include "Net/UnrealNetwork.h"
#include "Minigames/OBot/GameMode/MG_GameMode.h" //state 상태 받기 위함 
#include "Minigames/OBot/Character/MG_CharacterPlayer.h"
#include "Kismet/GameplayStatics.h"

void AMG_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	GameHUD = Cast<AGameHUD>(GetHUD());
	ServerCheckMatchState();
}

void AMG_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetHUDTime(DeltaTime);
	CheckTimeSync(DeltaTime);
}

void AMG_PlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMG_PlayerController, MatchState)
	DOREPLIFETIME(AMG_PlayerController, ServerTimeAtCoolDown)
	DOREPLIFETIME(AMG_PlayerController, bCoolDown)
}

float AMG_PlayerController::GetServerTime()
{
	//서버 권한이 있으면 그대로 아니면 client server delta time +@
	if (HasAuthority()) return GetWorld()->GetTimeSeconds();
	else return GetWorld()->GetTimeSeconds() + ClientServerDelta;
}

void AMG_PlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
	if (IsLocalController())
	{
		ServerRequesetServerTime(GetWorld()->GetTimeSeconds());
	}
}

void AMG_PlayerController::SetHUDMatchCountdown(float CountdownTime)
{
	//게임 진행중 시간 실시간 체크 
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

void AMG_PlayerController::SetHUDAnnouncementCountDown(float CountDownTime)
{
	//게임 시작전 announcement
	GameHUD = GameHUD == nullptr ? Cast<AGameHUD>(GetHUD()) : GameHUD;
	bool bHUDValid = GameHUD &&
		GameHUD->Announcement &&
		GameHUD->Announcement->WarmUpTime;
	if (bHUDValid)
	{
		if (CountDownTime < 0.f)
		{
			GameHUD->Announcement->WarmUpTime->SetText(FText());
			return;
		}
		int32 Minutes = FMath::FloorToInt(CountDownTime / 60.f);
		int32 Seconds = CountDownTime - Minutes * 60;

		FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		GameHUD->Announcement->WarmUpTime->SetText(FText::FromString(CountdownText));
	}
}

void AMG_PlayerController::SetHUDTime(float DeltaTime)
{
	AMG_GameMode* GameMode = Cast<AMG_GameMode>(GetWorld()->GetAuthGameMode()/*UGameplayStatics::GetGameMode(this)*/);
	float GetTimeSeconds = GetWorld()->GetTimeSeconds(); //실시간 
	float TimeLeft = 0.f; // 기록할 시간 
	if (MatchState == MatchState::WaitingToStart)
		TimeLeft = WarmupTime - GetServerTime() + LevelStartingTime;
	else if (MatchState == MatchState::InProgress)
		TimeLeft = GetServerTime() - WarmupTime - LevelStartingTime;
	else if (MatchState == MatchState::CoolDown && bCoolDown == false)
	{
		float GetTime = GetServerTime();
		ServerTimeAtCoolDown = GetTime;
		bCoolDown = true; // 플래그 설정
	}
	else if (MatchState == MatchState::CoolDown && bCoolDown == true) {
	TimeLeft = 5.0 + ServerTimeAtCoolDown + WarmupTime - GetServerTime();
	}
	/*else if (MatchState == MatchState::CoolDown && GameMode->bCoolDown == true) {
		TimeLeft = CoolDownTime +  RunTime - GetServerTime();
		/*TimeSinceLastLog += DeltaTime;
		if (TimeSinceLastLog >= 1.0f)
		{
			CoolDownTime -= DecreaseTime;
			TimeSinceLastLog = 0.0f;
		}
		
	}*/
	uint32 SecondsLeft = FMath::CeilToInt(TimeLeft);
	if (CountDown != SecondsLeft)
	{
		//SetHUDMatchCountdown(GetServerTime());
		if (MatchState == MatchState::WaitingToStart or MatchState == MatchState::CoolDown)
		{
			SetHUDAnnouncementCountDown(TimeLeft);
		}
		if (MatchState == MatchState::InProgress)
		{
			SetHUDMatchCountdown(TimeLeft);
		}
	}
}


void AMG_PlayerController::PollInit()
{
	//hud 실시간 갱신 
	if (MainWidget == nullptr)
	{
		if (GameHUD && GameHUD->MainWidget)
		{
			MainWidget = GameHUD->MainWidget;
		}
	}
}


void AMG_PlayerController::CheckTimeSync(float DeltaTime)
{
	TimeSyncRunningTime += DeltaTime; //시간 동기화 시간 계속 재설정
	if (IsLocalController() && TimeSyncRunningTime > TimeSyncFrequency)
	{
		ServerRequesetServerTime(GetWorld()->GetTimeSeconds());
		TimeSyncRunningTime = 0.f;
	}
}





void AMG_PlayerController::ServerRequesetServerTime_Implementation(float TimeOfClientRequest)
{
	float ServerTimeOfRecipt = GetWorld()->GetTimeSeconds();
	ClientReportServerTime(TimeOfClientRequest, ServerTimeOfRecipt);
}

void AMG_PlayerController::ClientReportServerTime_Implementation(float TimeOfClientRequest, float TimeServerReceivedClientRequest)
{
	float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest;
	float CurrentServerTime = TimeServerReceivedClientRequest + (0.5f * RoundTripTime);
	ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds();
}

void AMG_PlayerController::OnMatchStateSet(FName State)
{
	MatchState = State;
	if (MatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted();
	}
	if (MatchState == MatchState::CoolDown)
	{
		HandleCoolDown();
	}
	if (MatchState == MatchState::WaitingPostMatch)
	{
		HandleMatchHasEnded();
	}
}
//On_Rep 버그 해결 -> 멀티 함수 까지 매개변수를 가져올 필요는 x
void AMG_PlayerController::OnRep_MatchState()
{
	if (MatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted();
	}
	if (MatchState == MatchState::CoolDown)
	{
		HandleCoolDown();
	}
	if (MatchState == MatchState::WaitingPostMatch)
	{
		HandleMatchHasEnded();
	}
}

void AMG_PlayerController::OnRep_ServerTime()
{
	//float GetTimeSeconds = GetWorld()->GetTimeSeconds();
	//float TimeLeft = 0.f;
	//float RunTime = ServerTimeAtCoolDown;
    if (MatchState == MatchState::CoolDown && bCoolDown == false)
	{
		float GetTime = GetWorld()->GetTimeSeconds();
		ServerTimeAtCoolDown = GetTime;
		//bCoolDown = true; // 플래그 설정
	}
}



void AMG_PlayerController::HandleMatchHasStarted()
{
	GameHUD = GameHUD == nullptr ? Cast<AGameHUD>(GetHUD()) : GameHUD;
	if (GameHUD)
	{
		GameHUD->AddMainWidget();
		if (GameHUD->Announcement)
		{
			GameHUD->Announcement->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AMG_PlayerController::HandleMatchHasEnded()
{
	GameHUD = GameHUD == nullptr ? Cast<AGameHUD>(GetHUD()) : GameHUD;
	//AMG_CharacterPlayer* player = Cast<AMG_CharacterPlayer>(GetPawn());
	if (GameHUD)
	{
		if (GameHUD->Announcement)
		{
			GameHUD->Announcement->SetVisibility(ESlateVisibility::Hidden);
			//player->MainHUD->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AMG_PlayerController::HandleCoolDown()
{
	//cooldown상태면 -> ui 변경 
	GameHUD = GameHUD == nullptr ? Cast<AGameHUD>(GetHUD()) : GameHUD;
	if (GameHUD)
	{
		GameHUD->MainWidget->RemoveFromParent(); //굳이 제거 할 필요 있나 ?
		if (GameHUD->Announcement && GameHUD->Announcement->AnnouncementText && GameHUD->Announcement->InfoText)
		{
			GameHUD->Announcement->SetVisibility(ESlateVisibility::Visible);
			FString AnnouncementText("Hurry up!");
			GameHUD->Announcement->AnnouncementText->SetText(FText::FromString(AnnouncementText));
			GameHUD->Announcement->InfoText->SetText(FText::FromString(""));
		}
	}
}


void AMG_PlayerController::ServerCheckMatchState_Implementation()
{
	AMG_GameMode* GameMode = Cast<AMG_GameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
			WarmupTime = GameMode->WarmupTime;
			LevelStartingTime = GameMode->LevelStartingTime;
			CoolDownTime = GameMode->CoolDownAnnounceTime;
			MatchState = GameMode->GetMatchState();
			ClientJoinMidgame(MatchState, WarmupTime, CoolDownTime,LevelStartingTime);
	}
}

void AMG_PlayerController::ClientJoinMidgame_Implementation(FName StateOfMatch, float Warmup, float CoolDown, float StartingTime)
{
	WarmupTime = Warmup;
	LevelStartingTime = StartingTime;
	MatchState = StateOfMatch;
	OnMatchStateSet(MatchState);
	if (GameHUD && MatchState == MatchState::WaitingToStart)
	{
		GameHUD->AddAnnouncement();
	}
}

