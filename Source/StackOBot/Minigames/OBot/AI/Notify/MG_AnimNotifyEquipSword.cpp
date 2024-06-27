// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_AnimNotifyEquipSword.h"
#include "Minigames/OBot/AI/MG_EnemySkeleton.h"

void UMG_AnimNotifyEquipSword::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	Enemy = Cast<AMG_EnemySkeleton>(MeshComp->GetOwner());
	if (Enemy == nullptr)
	{
		return;
	}
	Enemy->SwordMesh->AttachToComponent(Enemy->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("RHand_Socket"));
}
