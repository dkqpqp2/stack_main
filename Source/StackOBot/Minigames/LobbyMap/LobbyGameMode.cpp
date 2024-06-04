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

void ALobbyGameMode::RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot)
{
	ABasePlayerState* PS = NewPlayer->GetPlayerState<ABasePlayerState>();
	if (!IsValid(PS))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("PS Is Not Valid : ALobbyGameMode::RestartPlayer()"));
		return;
	}

	ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(NewPlayer);
	if (!IsValid(PC))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("PC Is Not Valid : ALobbyGameMode::RestartPlayer()"));
		return;
	}

	// Change Game Instance to PlayerController...
	const TSubclassOf<APawn>* ClassOfSpawningPawn = PC->FindCharacterClass(PS->GetSelectedCharacter());

	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	auto SpawnedActor = GetWorld()->SpawnActor(*ClassOfSpawningPawn, &StartSpot->GetTransform(), SpawnParameters);
	APawn* SpawnedPawn = Cast<APawn>(SpawnedActor);
	if (!IsValid(SpawnedPawn))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("SpawnedPawn Not Valid : ALobbyGameMode::RestartPlayer()"));
		return;
	}
	NewPlayer->Possess(SpawnedPawn);


}

void ALobbyGameMode::PostSeamlessTravel()
{
	Super::PostSeamlessTravel();
	auto HostLobbyPC = GetWorld()->GetFirstPlayerController<ALobbyPlayerController>();
	if (!IsValid(HostLobbyPC))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("HOST LOBBY PC Not Valid : ALobbyGameMode::PostSeamlessTravel()"));
		return;
	}
	HostLobbyPC->LobbyWidgetUpdate();
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

	// 게임을 한번 진행 한 후 로비에 들어오는 경우 문제가 생길 여지가 있겠다.
	// 전체 플레이어 수를 가져 온 후 세팅하는게 맞을 지도 모르겠다.
	LobbyPS->SetPlayerEnterID(PlayerIDOfNextPlayer);

	if (LobbyPC->IsLocalController())	// Server's Controller
	{
		LobbyPS->SetIsHost(true);

	}


	auto HostLobbyPC = GetWorld()->GetFirstPlayerController<ALobbyPlayerController>();
	if (!IsValid(HostLobbyPC))
	{
		return;
	}

	// 방장이 아닌 새로운 플레이어가 들어올 때, 서버의 위젯을 한번 업데이트 시켜주자.
	// TODO : 그러고 보니 나갈때도 해줘야 겠네...
	if (PlayerIDOfNextPlayer != 0)
	{
		HostLobbyPC->LobbyWidgetUpdate();
	}

	PlayerIDOfNextPlayer++;
}

void ALobbyGameMode::ServerTravelToGameMap()
{
	GetWorld()->ServerTravel("TempGameMap");
}
