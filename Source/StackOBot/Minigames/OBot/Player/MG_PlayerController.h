// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../../GameMap/GamePlayerController.h"
#include "Net/UnrealNetwork.h"
#include "MG_PlayerController.generated.h"
/**
 * 
 */
UCLASS()
class STACKOBOT_API AMG_PlayerController : public AGamePlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual float GetServerTime(); //synced with server
	virtual void ReceivedPlayer() override; //sync with server clock as soon as possible
	void SetHUDMatchCountdown(float CountdownTime);
	void SetHUDAnnouncementCountDown(float CountDownTime);
	void SetHUDTime(float Deltatime);
	void PollInit();

	//sync time between client and server

	//Requests the current server time, passing in the client's time when the request was send
	UFUNCTION(Server,Reliable)
	void ServerRequesetServerTime(float TimeOfClientRequest); //클라에서 서버로 보내는 시간 
	//Client -> Server 로 request 요청 
	UFUNCTION(Client,Reliable)
	void ClientReportServerTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);

	float ClientServerDelta = 0.f; 

	//시간이 동기화가 잘됬는지 실시간 체크 
	UPROPERTY(EditAnywhere)
	float TimeSyncFrequency = 5.f;

	float TimeSyncRunningTime = 0.f;

	void CheckTimeSync(float DeltaTime);

	UFUNCTION(Server,Reliable)
	void ServerCheckMatchState();

	UFUNCTION(Client,Reliable)
	void ClientJoinMidgame(FName StateOfMatch,float Warmup,float CoolDown,float StartingTime); //match 제외

	UPROPERTY(ReplicatedUsing = OnRep_MatchState) //rpc 작업 매치상태 실시간 방송
	FName MatchState;


private:
    class AGameHUD* GameHUD;

	float MatchTime = 0.f; // 골인 지점 도착 후 시간 측정 
	uint32 CountDown = 0;

	UPROPERTY()
	class UMainWidget* MainWidget;

	UPROPERTY()
	class UAnnouncement* Announcement;


public:
	void OnMatchStateSet(FName State); //match모드 설정

	UFUNCTION()
	void OnRep_MatchState(); //Server 연결

	void HandleMatchHasStarted();
	void HandleCoolDown();

	float WarmupTime = 5.f;
	float LevelStartingTime = 0.f;

	float CoolDownTime = 5.0f;
	float RaceTIme = 0.f;

};
