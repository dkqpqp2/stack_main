// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MG_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API AMG_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMG_PlayerController();
private:
	TSubclassOf<UUserWidget> PlayerHUDClass;
	UUserWidget* PlayerHUD;

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
};
