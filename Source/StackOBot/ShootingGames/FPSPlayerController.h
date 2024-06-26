// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Minigames/GameMap/GamePlayerController.h"
#include "FPSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API AFPSPlayerController : public AGamePlayerController
{
	GENERATED_BODY()

public:
	void SetHUDHealth(float CurrentHealth, float MaxHealth, FString PlayerName);

	void SetHUDHealth_1(float CurrentHealth, float MaxHealth, FString PlayerName);

	void SetHUDHealth_2(float CurrentHealth, float MaxHealth, FString PlayerName);

	void SetHUDHealth_3(float CurrentHealth, float MaxHealth, FString PlayerName);

	void SetHUDAmmo(int32 Ammo, int32 MaxAmmo);

protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY()
	class AFPSHUD* FPSHUD;

	//Color
	UFUNCTION(BlueprintCallable, Category = "HealthColor")
	FLinearColor GetHealthColor(float HP);

	UPROPERTY(EditAnywhere)
	float Health;

	UPROPERTY(EditAnywhere, Category = "HealthColor")
	FLinearColor HealthColor;

	UPROPERTY(EditAnywhere, Category = "HealthColor")
	TObjectPtr<class UCurveLinearColor> HealthCurve;

	//Timer
	float MatchTime = 600.f;

public:
	void SetHUDMatchCountDown(float CountDownTume);
	uint32 CountDownInt = 0; // 0초가 될때 시간
	virtual void Tick(float DeltaTime) override;

	virtual float GetServerTime(); //sync with server world 
	virtual void ReceivedPlayer() override; //sync with server

protected:
	void SetHUDTime();
	/**
	*서버랑 클라 시간 맞추기 
	*/
	UFUNCTION(Server, Reliable)
	void ServerRequestServerTime(float TimeOfClientRequest);

	UFUNCTION(Client,Reliable)
	void ClientReportServerTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);

	float ClientServerDelta = 0.f; //difference between server and client 

	UPROPERTY(EditAnywhere)
	float TimeSyncFrequency = 5.f;

	float TimeSyncRunningTime = 0.f;
	void CheckTimeSync(float DeltaTime);

public:
	void OnMatchStateSet(FName State); //match모드 설정

	UFUNCTION()
	void OnRep_MatchState(); //Server 연결

	void HandleMatchHasStarted();
	void HandleMatchHasEnded();

	UPROPERTY(ReplicatedUsing = OnRep_MatchState) //rpc 작업 매치상태 실시간 방송
	FName MatchState;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UPROPERTY(Replicated,EditAnywhere)
	bool bIsWinner; //승패 bool

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> WinScreenClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> LoseScreenClass;


};
