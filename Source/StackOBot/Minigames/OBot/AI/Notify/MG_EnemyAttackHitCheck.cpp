// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_EnemyAttackHitCheck.h"
#include "Minigames/OBot/AI/MG_EnemyBase.h"

void UMG_EnemyAttackHitCheck::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AMG_EnemyBase* Enemy = Cast<AMG_EnemyBase>(MeshComp->GetOwner());
	Enemy->AttackHitCheck();
}

void UMG_EnemyAttackHitCheck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
