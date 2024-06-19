// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MG_EnemyBase.generated.h"

enum class EMonsterType
{
	Goblin,
	Ghoul,
	Skeleton,
	Zombie,
	Lich
};

UCLASS()
class STACKOBOT_API AMG_EnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMG_EnemyBase();

protected:
	virtual void BeginPlay() override;
	EMonsterType CurrentMonsterType;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewController) override;
};
