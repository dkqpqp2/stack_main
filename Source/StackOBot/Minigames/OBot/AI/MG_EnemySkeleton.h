// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Minigames/OBot/AI/MG_EnemyBase.h"
#include "MG_EnemySkeleton.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API AMG_EnemySkeleton : public AMG_EnemyBase
{
	GENERATED_BODY()

public:
	AMG_EnemySkeleton();

protected:
	virtual void BeginPlay() override;
	
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> SwordMesh;

	virtual void SetDead() override;

	UFUNCTION(NetMulticast, Unreliable)
	virtual void AttackByAI() override;

	UFUNCTION(NetMulticast, Unreliable)
	virtual void NotifyAttackActionEnd() override;
	
	bool IsEquipped;

	bool GetIsEquipped();
	UFUNCTION(NetMulticast, Reliable)
	void PlayEquipSwordMontage();
};
