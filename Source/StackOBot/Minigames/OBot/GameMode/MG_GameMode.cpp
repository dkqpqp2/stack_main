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

namespace MatchState
{
	const FName Cooldown = FName("CoolDown");
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
		CountDownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountDownTime <= 0.f)
		{
			StartMatch();
		}
	}

	if (MatchState == MatchState::InProgress) //조건을 다르게 해야됨 진행중인 상태에서 overlap이벤트 설정  Matchstate = Inprogress && overlap이벤트 발생시 cooldown으로 
	{
		CoolDownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime; // 쿨다운 타임 설정 -> 오버랩이벤트 설정  
	}
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
		if (PlayerCharacter)
		{
			PlayerCharacter->OnFinishLineReached.AddDynamic(this, &AMG_GameMode::OnPlayerFinishLineReached);
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
		PlayerState->GetPawn()->SetActorTransform(PodiumActor->PlayerLocations[i]->GetComponentTransform());		

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
}


void AMG_GameMode::BeginPlay()
{
	Super::BeginPlay();

	LevelStartingTime = GetWorld()->GetTimeSeconds();
}

void AMG_GameMode::OnPlayerFinishLineReached()
{
	// 플레이어가 결승선에 도달했을 때의 처리를 여기에 추가합니다.
	// 매치 상태를 쿨다운으로 변경하거나 추가 작업을 수행할 수 있습니다.
	//SetMatchState(MatchState::Cooldown);
	UE_LOG(LogTemp, Warning, TEXT("CoolDown"));
}