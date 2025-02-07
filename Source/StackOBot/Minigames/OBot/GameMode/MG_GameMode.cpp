// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_GameMode.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameState.h"
#include "Minigames/OBot/Player/MG_PlayerController.h"
#include "Minigames/GameMap/GamePlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "../Actors/Podium.h"
#include "Minigames/OBot/Character/MG_CharacterPlayer.h"
#include "Minigames/Item/FinishLineBox.h"
#include "Minigames/OBot/PlayerState/MG_PlayerState.h"

namespace MatchState
{
	const FName CoolDown = FName("CoolDown"); // 카운트다운
}

AMG_GameMode::AMG_GameMode()
{
	bDelayedStart = true; // 처음에 대기 상태 <날라다니기>

	static ConstructorHelpers::FClassFinder<APawn> OBotClassRef(TEXT("/Game/Character/Animation/BPMG_CharacterPlayer.BPMG_CharacterPlayer_C"));
	if (OBotClassRef.Class)
	{
		DefaultPawnClass = OBotClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<AMG_PlayerController> PlayerControllerClassRef(TEXT("/Script/StackOBot.MG_PlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

}

void AMG_GameMode::Tick(float Deltatime)
{
	Super::Tick(Deltatime);

	if (MatchState == MatchState::WaitingToStart)
	{
		float GetTimeSeconds = GetWorld()->GetTimeSeconds();
		CountDownTime = WarmupTime - GetTimeSeconds + LevelStartingTime; 
		if (CountDownTime <= 0.f)
		{
			StartMatch();
		}
	}
	else if (MatchState == MatchState::CoolDown)
	{
		if (bCoolDown == false)
		{
			float GetTimeSeconds = GetWorld()->GetTimeSeconds();
			// 한 번만 저장하도록 플래그를 사용하여 체크
			ServerTimeAtCoolDown = GetTimeSeconds;
			bCoolDown = true; // 플래그 설정
		}
		if(bCoolDown == true)
		CountDownTime = CoolDownAnnounceTime + ServerTimeAtCoolDown/*게임진행시간*/ - GetWorld()->GetTimeSeconds();
		if (CountDownTime <= 0.f)
		{
			UE_LOG(LogTemp, Warning, TEXT("post"));
			SetMatchState(MatchState::WaitingPostMatch);
		}
	}
	//결승전에 도착하는건 broadcast로 처리 할 것

}

void AMG_GameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AMG_CharacterPlayer* PlayerCharacter = Cast<AMG_CharacterPlayer>((*It)->GetPawn());
		AMG_PlayerController* MGPlayer = Cast<AMG_PlayerController>(*It);
		if (MGPlayer)
		{
			MGPlayer->OnMatchStateSet(MatchState);
		}
		//if (PlayerCharacter)
		//{
		//	PlayerCharacter->OnFinishLineReached.AddDynamic(this, &AMG_GameMode::OnPlayerFinishLineReached);
		//}
	}

	if (MatchState == MatchState::InProgress)
	{
		// 시작.
		GetWorldTimerManager().SetTimer(UpdatePlayersRankTimer, this, &ThisClass::UpdatePlayersRank, 0.1f, true, 3.f);
	}
}

void AMG_GameMode::HandleMatchHasEnded()
{
	// GameMode::HandleMatchHasEnded()에서 아예 온라인 세션을 끊어버리려고 하는데... 
	// 테스트 결과 매치가 끝나고 이 부모함수들이 호출되어도 클라이언트가 나가거나 하지 않는다.
	// 안심해도 되겠다.
	Super::HandleMatchHasEnded();
	
	GetWorldTimerManager().ClearTimer(UpdatePlayersRankTimer);

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

	int i = 0;
	for (TObjectPtr<APlayerState> PlayerState : GS->PlayerArray)
	{
		if (i >= PodiumActor->PlayerLocations.Num())
		{
			i = PodiumActor->PlayerLocations.Num() - 1;
		}
		// 폰 위치 이동.
		// 왜 클라이언트와 서버의 위치가 살짝 다르지? (2등위치)

		AMG_PlayerState* MGPS = Cast<AMG_PlayerState>(PlayerState);
		if (1 <= MGPS->GetFinalRank() && MGPS->GetFinalRank() <= 4)
		{
			PlayerState->GetPawn()->SetActorTransform(PodiumActor->PlayerLocations[MGPS->GetFinalRank() - 1]->GetComponentTransform());
		}

		auto PC = PlayerState->GetPlayerController();
		if (IsValid(PC))
		{
			// 알아서 PodiumActor안에 있는 카메라 컴포넌트를 찾아서 하는 viewtarget을 바꾸는듯...
			PC->SetViewTarget(PodiumActor);
			PlayerState->GetPawn<AMG_CharacterPlayer>()->OnDisableInput();
		}

		// 각 플레이어의 등수에 맞게 이동시켜야겠다.(TODO)
		i++;
	}

	// 몇초 뒤에 시상식 종료... 로비화면으로 이동?
	GetWorldTimerManager().SetTimer(FinishLevelTimer, this, &ThisClass::ReturnToLobby, 5.f, false);

}

void AMG_GameMode::UpdatePlayersRank()
{
	AGameState* GS = GetGameState<AGameState>();
	if (!IsValid(GS))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("GameState is Not Valid (AMG_GameMode::UpdatePlayersRank())"));
		return;
	}

	if (!IsValid(FinishActor))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("FinishActor is Not Valid (AMG_GameMode::UpdatePlayersRank())"));
		return;
	}
	if ((GS->GetMatchState() != MatchState::InProgress) && (GS->GetMatchState() != MatchState::CoolDown))
	{
		return;
	}


	TArray<TTuple<TObjectPtr<APlayerState>, float>> PlayerStateWithDistance;
	for (TObjectPtr<APlayerState> PlayerState : GS->PlayerArray)
	{
		float Distance = PlayerState->GetPawn()->GetSquaredDistanceTo(FinishActor);
		PlayerStateWithDistance.Add(MakeTuple(PlayerState, Distance));
	}

	PlayerStateWithDistance.Sort(
		[](const TTuple<TObjectPtr<APlayerState>, float> A, const TTuple<TObjectPtr<APlayerState>, float> B) 
		{
			return A.Value < B.Value;
		}
	);

	int i = 1;
	for (TTuple<TObjectPtr<APlayerState>, float> PSTuple : PlayerStateWithDistance)
	{
		auto PS = PSTuple.Key;
		Cast<AMG_PlayerState>(PS)->SetRank(i);
		i++;
	}

	// 순위UI업데이트...
}

void AMG_GameMode::ReturnToLobby()
{
	GetWorld()->ServerTravel("/Game/MainGame/Level/LobbyMap");

}


void AMG_GameMode::BeginPlay()
{
	Super::BeginPlay();

	LevelStartingTime = GetWorld()->GetTimeSeconds();

	// 결승선 찾아놓기.
	// 지금은 임시 액터로 대체 해놓자.
	TArray<AActor*> FinishActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Finish"), FinishActors);
	if (FinishActors.Num() > 0)
	{
		FinishActor = FinishActors[0];

	}


}

void AMG_GameMode::OnPlayerFinishLineReached()
{
	// 플레이어가 결승선에 도달했을 때의 처리를 여기에 추가합니다.
	// 매치 상태를 쿨다운으로 변경하거나 추가 작업을 수행할 수 있습니다.
	//EndMatch();
	SetMatchState(MatchState::CoolDown);//여기서 HasMatchEnded -> 쿨타운 으로 바꾼다음에 끝나면 postmatch로 해야함 
	UE_LOG(LogTemp, Warning, TEXT("CoolDown"));
}
