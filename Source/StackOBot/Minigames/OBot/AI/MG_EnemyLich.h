// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Minigames/OBot/AI/MG_EnemyBase.h"
#include "MG_EnemyLich.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API AMG_EnemyLich : public AMG_EnemyBase
{
	GENERATED_BODY()

public:
	AMG_EnemyLich();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> PoisionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> SpawnMontage;

	void PoisonSkill(APawn* InClosePawn);
	void SpawnSkill(APawn* InClosePawn);

	UFUNCTION(NetMulticast, Unreliable)
	void PoisonSkillAnimation();
	UFUNCTION(NetMulticast, Unreliable)
	void SpawnSkillAnimation();

	float SkillDetectRange = 5000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AMG_LichPoison> LichPoisonClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpawnEnemy)
	TSubclassOf<class AMG_EnemyBase> SpawnEnemyClass;

	APawn* FindTarget();

public:
	virtual void Tick(float DeltaTime) override;

public:
	virtual void SetDead() override;
	UFUNCTION(NetMulticast, Unreliable)
	virtual void AttackByAI() override;

	UFUNCTION(NetMulticast, Unreliable)
	virtual void NotifyAttackActionEnd() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill)
	float MaxPoisonCoolTime;
	float CurrentPoisonCoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill)
	float MaxSpawnCoolTime;
	float CurrentSpawnCoolTime;

};
