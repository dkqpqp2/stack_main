// Copyright Epic Games, Inc. All Rights Reserved.

#include "LobbyGameMode.h"
#include "../ThirdPersonCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "LobbyPlayerController.h"
#include "LobbyPlayerState.h"
#include "LobbyGameStateBase.h"

ALobbyGameMode::ALobbyGameMode()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	//if (PlayerPawnBPClass.Class != NULL)
	//{
	//	DefaultPawnClass = PlayerPawnBPClass.Class;
	//}

	//PlayerControllerClass = ALobbyPlayerController::StaticClass();
	//PlayerStateClass = ALobbyPlayerState::StaticClass();
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto LobbyPC = Cast<ALobbyPlayerController>(NewPlayer);
	if (!IsValid(LobbyPC))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("OnPostLogin, LobbyPlayerController Not Valid"));
		return;
	}
	auto LobbyPS = LobbyPC->GetPlayerState<ALobbyPlayerState>();
	if (!IsValid(LobbyPS))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("OnPostLogin, LobbyPlayerState Not Valid"));
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, TEXT("OnPostLogin, LobbyPlayerState Available?")); // It Seems Okay.


	auto LobbyGS = GetGameState<ALobbyGameStateBase>();
	if (!IsValid(LobbyGS))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("OnPostLogin, GameState Not Valid"));
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("%d : Current GameState's PlayerArray's Num"), LobbyGS->PlayerArray.Num())); // It Seems Okay 2

	LobbyPS->SetPlayerEnterID(PlayerIDOfNextPlayer);


	auto LocalLobbyPC = GetWorld()->GetFirstPlayerController<ALobbyPlayerController>();
	if (!IsValid(LocalLobbyPC))
	{
		return;
	}

	// 방장이 아닌 새로운 플레이어가 들어올 때, 서버의 위젯을 한번 업데이트 시켜주자.
	// TODO : 그러고 보니 나갈때도 해줘야 겠네...
	if (PlayerIDOfNextPlayer != 0)
	{
		LocalLobbyPC->LobbyWidgetUpdate();
	}

	PlayerIDOfNextPlayer++;
}

void ALobbyGameMode::ServerTravelToGameMap()
{
	GetWorld()->ServerTravel("TempGameMap");
}
