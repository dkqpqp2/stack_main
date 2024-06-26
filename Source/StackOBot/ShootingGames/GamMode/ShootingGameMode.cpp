// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingGameMode.h"
#include "../../Minigames/OBot/Character/MG_ShootingCharacterPlayer.h"
#include"../StackOBot/ShootingGames/FPSPlayerController.h"
#include "Minigames/OBot/Actors/Podium.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerState.h"


void AShootingGameMode::BeginPlay()
{
	Super::BeginPlay();
}


void AShootingGameMode::OnPlayerFinishLineReached()
{
	IsWin = true; //승리했다는 BOOL 값 
	SetMatchState(MatchState::WaitingPostMatch);
	UE_LOG(LogTemp, Warning, TEXT("WaitingPostMatch"));
	//로비로 돌아가는 code 
}

void AShootingGameMode::AllPlayerDeadCheck()
{
	bool HPCheck = true;
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AMG_ShootingCharacterPlayer* PlayerCharacter = Cast<AMG_ShootingCharacterPlayer>((*It)->GetPawn());
		if (PlayerCharacter->CurrentHealth <= 0.f)
		{
			//넘기다가 모두다 그러면 OnMatchState == PostWait
			continue;
		}
		else
		{
			HPCheck = false; 
			break;
		}
	}
	//true라면 모든 플레이어 체력이 0이라는 것 
	if (HPCheck)
	{
		//이때 Dead ui를 보여줘야 하니까 mode 자체 bool값으로 승리인지 패배인지 하는값이 필요함 
		IsWin = false;
		SetMatchState(MatchState::WaitingPostMatch); 
	}
}

void AShootingGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AMG_ShootingCharacterPlayer* PlayerCharacter = Cast<AMG_ShootingCharacterPlayer>((*It)->GetPawn());
		AFPSPlayerController* MGPlayer = Cast<AFPSPlayerController>(*It); // 각 컨트롤러 플레이어 별 MATCHSTATE 세팅 
		if (MGPlayer)
		{
			MGPlayer->OnMatchStateSet(MatchState);
		}
		if (PlayerCharacter)
		{
			PlayerCharacter->OnFinishLineReached1.AddDynamic(this, &AShootingGameMode::OnPlayerFinishLineReached); //결승전에 도달한거 전달받으면 reached함수 
		}
	}
}

void AShootingGameMode::HandleMatchHasEnded()
{
	//Poduim처리 여기서 해주시면 됩니당 
	//승리시 성공한 캐릭터들만 실패시 아무도 안나오는 식으로 ? 
	Super::HandleMatchHasEnded();

	// podium을 가져오자.
	APodium* PodiumActor = Cast<APodium>(UGameplayStatics::GetActorOfClass(GetWorld(), APodium::StaticClass()));
	if (!IsValid(PodiumActor))
	{
		return;
	}

	AGameState* GS = GetGameState<AGameState>();
	if (!IsValid(GS))
	{
		return;
	}

	int i = 1;
	for (TObjectPtr<APlayerState> PlayerState : GS->PlayerArray)
	{
		if (i >= PodiumActor->PlayerLocations.Num())
		{
			i = PodiumActor->PlayerLocations.Num() - 1;
		}
		// 폰 위치 이동.
		// 왜 클라이언트와 서버의 위치가 살짝 다르지? (2등위치)

		AMG_ShootingCharacterPlayer* Pawn = PlayerState->GetPawn<AMG_ShootingCharacterPlayer>();
		if (Pawn->IsWinner)
		{
			PlayerState->GetPawn()->SetActorTransform(PodiumActor->PlayerLocations[0]->GetComponentTransform());
		}
		else
		{
			if (i >= 4)
			{
				i = 3;
			}
			PlayerState->GetPawn()->SetActorTransform(PodiumActor->PlayerLocations[i]->GetComponentTransform());
			i += 1;
		}
		

		auto PC = PlayerState->GetPlayerController();
		if (IsValid(PC))
		{
			// 알아서 PodiumActor안에 있는 카메라 컴포넌트를 찾아서 하는 viewtarget을 바꾸는듯...
			PC->SetViewTarget(PodiumActor);
			//PlayerState->GetPawn<AMG_CharacterPlayer>()->OnDisableInput();
		}

	}

	// 몇초 뒤에 시상식 종료... 로비화면으로 이동?
	GetWorldTimerManager().SetTimer(FinishLevelTimer, this, &ThisClass::ReturnToLobby, 5.f, false);
}

void AShootingGameMode::ReturnToLobby()
{
	GetWorld()->ServerTravel("/Game/Lobby/ThirdPerson/Maps/LobbyMap");

}

