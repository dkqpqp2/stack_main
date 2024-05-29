// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMapGameMode.h"
#include "../BasePlayerState.h"
#include "../BaseGameInstance.h"
#include "GamePlayerController.h"


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
}
