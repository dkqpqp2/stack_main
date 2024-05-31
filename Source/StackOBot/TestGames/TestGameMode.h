// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Minigames/LobbyMap/LobbyGameMode.h"
#include "TestGameMode.generated.h"

/**
 * 
 */
UCLASS()

class STACKOBOT_API ATestGameMode : public ALobbyGameMode
{
	GENERATED_BODY()

public:
	ATestGameMode();

protected:
	int32   mItemCount = 9;
	int32   mTeamNumber = 0;
public:
	void  PickUpItem()
	{
		--mItemCount;
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Item Count : %d"), mItemCount);
	}
	
protected:
	virtual void BeginPlay();


};
