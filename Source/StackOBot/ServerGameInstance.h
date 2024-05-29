// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "ServerGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FServerInfo //서버 정보 가져오기 위한 구조체 
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	   FString ServerName;
    UPROPERTY(BlueprintReadOnly)
	   FString PlayerCountStr;
	UPROPERTY(BlueprintReadOnly)
	   int32 CurrentPlayers;
	UPROPERTY(BlueprintReadOnly)
	   int32 MaxPlayers;

	   void SetPlayerCount()
	   {
		   PlayerCountStr = FString(FString::FromInt(CurrentPlayers) + "/" + FString::FromInt(MaxPlayers));
	   } 
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerDel, FServerInfo, ServerListDel);
UCLASS()
class STACKOBOT_API UServerGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UServerGameInstance();

protected:
	UPROPERTY(BlueprintAssignable)
	FServerDel ServerListDel;

	IOnlineSessionPtr SessionInterface;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;


	virtual void Init() override;

	virtual void OnCreateSessionComplete(FName ServerName, bool Succeeded);
	virtual void OnFindSessionComplete(bool Succeeded);
	virtual void OnJoinsessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	UFUNCTION(BlueprintCallable)
	void CreateServer(FString ServerName, FString HostName);

	UFUNCTION(BlueprintCallable)
	void FindServers();
};
