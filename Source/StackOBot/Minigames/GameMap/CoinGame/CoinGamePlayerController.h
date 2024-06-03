// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Minigames/GameMap/GamePlayerController.h"
#include "CoinGamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API ACoinGamePlayerController : public AGamePlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	
	// 블루프린트에서 이 클래스 지정.
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UUserWidget> CoinGameWidgetClass;
protected:
	class UCoinGameWidget* CoinGameWidget;
	
public:
	void SetTeamScoreToWidget(bool IsRedTeam, int NewTeamScore);
};
