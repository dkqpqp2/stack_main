// Fill out your copyright notice in the Description page of Project Settings.


#include "Minigames/OBot/AI/Notify/MG_EnemyAttackNotify.h"
#include "Minigames/OBot/AI/MG_EnemyBase.h"

void UMG_EnemyAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	Enemy = Cast<AMG_EnemyBase>(MeshComp->GetOwner());
	if (Enemy == nullptr)
	{
		return;
	}
	Enemy->AttackHitCheck();
}
