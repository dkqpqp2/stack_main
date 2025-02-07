// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MG_EnemyBase.h"
#include "MG_EnemyGoblin.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API AMG_EnemyGoblin : public AMG_EnemyBase
{
	GENERATED_BODY()

public:
	AMG_EnemyGoblin();

protected:
	virtual void BeginPlay() override;
public:
	virtual void SetDead() override;

	UFUNCTION(NetMulticast, Unreliable)
	virtual void AttackByAI() override;

	UFUNCTION(NetMulticast, Unreliable)
	virtual void NotifyAttackActionEnd() override;

	
};
