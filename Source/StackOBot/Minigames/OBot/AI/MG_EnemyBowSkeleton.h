// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Minigames/OBot/AI/MG_EnemyBase.h"
#include "MG_EnemyBowSkeleton.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API AMG_EnemyBowSkeleton : public AMG_EnemyBase
{
	GENERATED_BODY()
public:
	AMG_EnemyBowSkeleton();

protected:
	virtual void BeginPlay() override;
	void FindTarget();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Bow)
	TObjectPtr<class USkeletalMeshComponent> BowMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Arrow)
	TSubclassOf<class AMG_SkeletonArrow> ArrowClass;

	virtual void SetDead() override;

	UFUNCTION(NetMulticast, Unreliable)
	virtual void AttackByAI() override;

	UFUNCTION(NetMulticast, Unreliable)
	virtual void NotifyAttackActionEnd() override;



	
};
