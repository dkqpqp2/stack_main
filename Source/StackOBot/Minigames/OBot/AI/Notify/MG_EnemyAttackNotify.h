// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "MG_EnemyAttackNotify.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API UMG_EnemyAttackNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	TObjectPtr<class AMG_EnemyBase> Enemy;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);
};
