// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_GameMode.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameState.h"
#include "Minigames/OBot/Player/MG_PlayerController.h"
#include "Minigames/GameMap/GamePlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "../Actors/Podium.h"


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
		CountDownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountDownTime <= 0.f)
		{
			StartMatch();
		}
	}
}

void AMG_GameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AMG_PlayerController* MGPlayer = Cast<AMG_PlayerController>(*It);
		if (MGPlayer)
		{
			MGPlayer->OnMatchStateSet(MatchState);
		}
	}
}

void AMG_GameMode::HandleMatchHasEnded()
{
	// GameMode::HandleMatchHasEnded()에서 아예 온라인 세션을 끊어버리려고 하는데... 
	// 테스트 결과 매치가 끝나고 이 부모함수들이 호출되어도 클라이언트가 나가거나 하지 않는다.
	// 안심해도 되겠다.
	Super::HandleMatchHasEnded();
	


	// podium을 가져오자.
	APodium* PodiumActor = Cast<APodium>(UGameplayStatics::GetActorOfClass(GetWorld(), APodium::StaticClass()));
	if (!IsValid(PodiumActor))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Podium is Not Valid (AMG_GameMode::HandleMatchHasEnded())"));
		return;
	}

	AGameState* GS = GetGameState<AGameState>();
	if (!IsValid(GS))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("GameState is Not Valid (AMG_GameMode::HandleMatchHasEnded())"));
		return;
	}

	int i = 0;
	for (TObjectPtr<APlayerState> Player : GS->PlayerArray)
	{
		if (i >= PodiumActor->PlayerLocations.Num())
		{
			i = PodiumActor->PlayerLocations.Num() - 1;
		}
		Player->GetPawn()->SetActorTransform(PodiumActor->PlayerLocations[i]->GetComponentTransform());
		// 각 폰의 함수 호출...(포디움의 위치정보 전달..., 카메라의 위치정보 또는 포인터 전달...)
		//Player->GetPlayerController()->SetViewTarget();
		auto PC = Player->GetPlayerController();
		if (IsValid(PC))
		{
			// 알아서 PodiumActor안에 있는 카메라 컴포넌트를 찾아서 하는 viewtarget을 바꾸는듯...
			PC->SetViewTarget(PodiumActor);
		}

		// 각 플레이어의 등수에 맞게 이동시켜야겠다.(TODO)
		i++;
	}


	// 모든 캐릭터 플레이어를 가져와서
	//GetGameState<AGameState>()->PlayerArray[0]->GetPawn();
	
	// 각각 캐릭터의 (시상식으로 (등수에 맞게) 이동시키고 카메라를 전환시키고, 입력비활성화하는)함수를 호출

	// 몇초 뒤에 시상식 종료... 로비화면으로 이동?
}


void AMG_GameMode::BeginPlay()
{
	Super::BeginPlay();

	LevelStartingTime = GetWorld()->GetTimeSeconds();
}

