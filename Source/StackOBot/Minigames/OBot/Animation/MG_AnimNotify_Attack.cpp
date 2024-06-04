// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_AnimNotify_Attack.h"
#include "Minigames/OBot/Projectile/MG_PlayerProjectile.h"
#include "Minigames/OBot/Interface/MG_AnimationAttackInterface.h"

void UMG_AnimNotify_Attack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IMG_AnimationAttackInterface* AttackPawn = Cast<IMG_AnimationAttackInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->AttackHitCheck();
		}

	}

	/*if (MeshComp && MeshComp->GetOwner())
	{
		FVector SocketLocation = MeshComp->GetSocketLocation(TEXT("AttackSocket"));
		FRotator SocketRotation = MeshComp->GetSocketRotation(TEXT("AttackSocket"));

		UWorld* World = MeshComp->GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = MeshComp->GetOwner();
			SpawnParams.Instigator = MeshComp->GetOwner()->GetInstigator();

			World->SpawnActor<AMG_PlayerProjectile>(AMG_PlayerProjectile::StaticClass(), SocketLocation, SocketRotation, SpawnParams);
		}

	}*/
}
