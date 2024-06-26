// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Minigames/OBot/AI/MG_EnemyBase.h"
#include "MG_EnemyZombie.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API AMG_EnemyZombie : public AMG_EnemyBase
{
	GENERATED_BODY()
public:
	AMG_EnemyZombie();

protected:
	virtual void BeginPlay() override;
public:
	virtual void SetDead() override;

	UFUNCTION(NetMulticast, Unreliable)
	virtual void AttackByAI() override;

	UFUNCTION(NetMulticast, Unreliable)
	virtual void NotifyAttackActionEnd() override;
};
