// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MG_NPCController.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API AMG_NPCController : public AAIController
{
	GENERATED_BODY()

public:
	AMG_NPCController();
	virtual void OnPossess(APawn* InPawn) override;
	//virtual void OnUnPossess() override;

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;
private:
	/*void OnRepeatTimer();

	FTimerHandle RepeatTimerHandle;
	float RepeatInterval;*/

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;
	
};
