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
	void RunAI();
	void StopAI();

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;
private:


	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTreeComponent> BTComponent;
};
