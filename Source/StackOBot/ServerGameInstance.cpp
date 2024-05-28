// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerGameInstance.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

UServerGameInstance::UServerGameInstance()
{
}

void UServerGameInstance::Init()
{
	if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			//Bind Delegate here
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UServerGameInstance::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UServerGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UServerGameInstance::OnJoinsessionComplete);
 		    
		}
	}
}

void UServerGameInstance::OnCreateSessionComplete(FName ServerName, bool Succeeded)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete, Succeede : %d"), Succeeded);
	if (Succeeded)
	{
		GetWorld()->ServerTravel("/Game/MainGame/Level/Lobby_PR?listen");
	}
}

void UServerGameInstance::OnFindSessionComplete(bool Succeeded)
{
	UE_LOG(LogTemp, Warning, TEXT("OnFindSession Complete, Succees : %d"), Succeeded);
	if (Succeeded)
	{
		TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;
		for (FOnlineSessionSearchResult Result : SearchResults)
		{
			if (!Result.IsValid())
				continue;
			FServerInfo Info;
			Info.ServerName = "Test Server name";
			Info.MaxPlayers = Result.Session.SessionSettings.NumPublicConnections;
			Info.CurrentPlayers = Info.MaxPlayers - Result.Session.NumOpenPublicConnections;
			ServerListDel.Broadcast(Info);

		}

		UE_LOG(LogTemp, Warning, TEXT("OnFindSession Complete, Server Count : %d"), SearchResults.Num());

		if (SearchResults.Num())
		{
			UE_LOG(LogTemp, Warning, TEXT("OnJoinSession"));
			SessionInterface->JoinSession(0, "My Session", SearchResults[0]);
		}

	}
}

void UServerGameInstance::OnJoinsessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete, SessionName : %s"), *SessionName.ToString());
	if(APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FString JoinAddress = "";
		SessionInterface -> GetResolvedConnectString(SessionName, JoinAddress);
		if (JoinAddress != "")
			PController->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
	}
}

void UServerGameInstance::CreateServer()
{
	UE_LOG(LogTemp, Warning, TEXT("CreateServer"));

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	if (IOnlineSubsystem::Get()->GetSubsystemName() != "NULL")
		SessionSettings.bIsLANMatch = false;
	else
		SessionSettings.bIsLANMatch = true;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.NumPublicConnections = 5;

	SessionInterface->CreateSession(0, FName("My Session"), SessionSettings);

}

void UServerGameInstance::JoinServer()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch()); //MakeShareable = smart_pointer 
	if (IOnlineSubsystem::Get()->GetSubsystemName() != "NULL")
		SessionSearch -> bIsLanQuery = false;
	else
		SessionSearch -> bIsLanQuery = true;
	SessionSearch->MaxSearchResults = 10000; // 검색 결과 10000개
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);// query setting은 조건검색 -> prsence는 접속 여부로 보면됩니다  equals -> 이에 해당하는

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	UE_LOG(LogTemp, Warning, TEXT("JoinServer"));
}
