// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameMapGameMode.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API AGameMapGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	AGameMapGameMode();
	
public:
	virtual void RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot) override;

	// is it playercontroller changing test.
	virtual void SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC) override;

protected:
	//virtual void OnMatchStateSet() override;
	virtual void HandleMatchHasEnded() override;



	
};
