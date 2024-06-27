// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "MG_AnimNotifyEquipSword.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API UMG_AnimNotifyEquipSword : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	TObjectPtr<class AMG_EnemySkeleton> Enemy;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);
};
