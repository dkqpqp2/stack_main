// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMapGameMode.h"
#include "../BasePlayerState.h"
#include "../BaseGameInstance.h"
#include "GamePlayerController.h"


AGameMapGameMode::AGameMapGameMode()
{
	for (auto ItemClass : ItemClasses)
	{
		AItemBase* Item = NewObject<AItemBase>(this, ItemClass);
		AvailableItems.Add(Item);
	}
}

void AGameMapGameMode::RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot)
{
	ABasePlayerState* PS = NewPlayer->GetPlayerState<ABasePlayerState>();
	if (!IsValid(PS))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("PS Is Not Valid : AGameMapGameMode::RestartPlayer()"));
		return;
	}

	AGamePlayerController* PC = Cast<AGamePlayerController>(NewPlayer);
	if (!IsValid(PC))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("PC Is Not Valid : AGameMapGameMode::RestartPlayer()"));
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
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("SpawnedPawn Not Valid : AGameMapGameMode::RestartPlayer()"));
		return;
	}
	NewPlayer->Possess(SpawnedPawn);
	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Green, NewPlayer->GetName());
}

void AGameMapGameMode::SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("---SwapPlayerController"));
	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Blue, OldPC->GetName());
	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, NewPC->GetName());
	// OldPC will be destroy...
	Super::SwapPlayerControllers(OldPC, NewPC);

	// 이전 플레이어컨트롤러 파괴?
	if (IsValid(OldPC))
	{
		if (GetWorld()->DestroyActor(OldPC))
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("--- Destroy Old PC---"));
			// 실험 결과 파괴해도 다시 생성되는듯 하다... 어디서? 어떻게????
		}
	}
}

void AGameMapGameMode::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();

	GetWorld()->ServerTravel("/Game/Lobby/ThirdPerson/Maps/LobbyMap");
}

AItemBase* AGameMapGameMode::GetItem()
{
	if (AvailableItems.IsEmpty())
	{
		return nullptr;
	}

	
	// 우선은 0번아이템 전달.
	return AvailableItems[0];
}

