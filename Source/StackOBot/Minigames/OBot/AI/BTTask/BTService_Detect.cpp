// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "Minigames/OBot/Player/MG_NPCController.h"
#include "Minigames/OBot/Character/MG_ShootingCharacterPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "Minigames/OBot/AI/MG_EnemyBase.h"
#include "Minigames/OBot/AI/MG_EnemySkeleton.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Minigames/OBot/Interface/MG_AIInterface.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();

	if (nullptr == World)
	{
		return;
	}

	IMG_AIInterface* AIPawn = Cast<IMG_AIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return;
	}
	float DetectRadius = AIPawn->GetAIDetectRange();

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	APawn* ClosePawn = nullptr;
	float MinDistanceSquared = FLT_MAX;
	AMG_EnemyBase* Enemy = Cast<AMG_EnemyBase>(ControllingPawn);
	UAnimInstance* AnimInstance = Cast<UAnimInstance>(Enemy->GetMesh()->GetAnimInstance());
	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			if (Pawn && Pawn->GetController()->IsPlayerController()) // 
			{
				float DistanceSquared = FVector::DistSquared(Pawn->GetActorLocation(), Center);

				if (DistanceSquared < MinDistanceSquared)
				{
					MinDistanceSquared = DistanceSquared;
					ClosePawn = Pawn;
				}
			}
		}
	}
	AMG_EnemySkeleton* Skeleton = Cast<AMG_EnemySkeleton>(Enemy);
	if (ClosePawn)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMG_NPCController::TargetKey, ClosePawn);
		
		if (Enemy)
		{
			switch (Enemy->GetCurrentMonsterType())
			{
			case EMonsterType::Goblin:
				Enemy->GetCharacterMovement()->MaxWalkSpeed = 450.f;
				break;
			case EMonsterType::Ghoul:
				Enemy->GetCharacterMovement()->MaxWalkSpeed = 500.f;
				break;
			case EMonsterType::Skeleton:
				Enemy->GetCharacterMovement()->MaxWalkSpeed = 450.f;
				if (Skeleton)
				{
					if (!Skeleton->GetIsEquipped())
					{
						Skeleton->PlayEquipSwordMontage();
					}
				}
				break;
			case EMonsterType::Zombie:
				Enemy->GetCharacterMovement()->MaxWalkSpeed = 230.f;
				break;
			case EMonsterType::Lich:
				Enemy->GetCharacterMovement()->MaxWalkSpeed = 300.f;
				break;
			default:
				break;
			}
		}
		DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
		DrawDebugLine(World, ControllingPawn->GetActorLocation(), ClosePawn->GetActorLocation(), FColor::Blue, false, 0.2f);
	}
	else
	{
		if (Enemy)
		{
			switch (Enemy->GetCurrentMonsterType())
			{
			case EMonsterType::Goblin:
				Enemy->GetCharacterMovement()->MaxWalkSpeed = 300.0f;
				break;
			case EMonsterType::Ghoul:
				Enemy->GetCharacterMovement()->MaxWalkSpeed = 230.f;
				break;
			case EMonsterType::Skeleton:
				Enemy->GetCharacterMovement()->MaxWalkSpeed = 300.f;
				break;
			case EMonsterType::Zombie:
				Enemy->GetCharacterMovement()->MaxWalkSpeed = 130.0f;
				break;
			case EMonsterType::Lich:
				Enemy->GetCharacterMovement()->MaxWalkSpeed = 300.f;
				break;
			default:
				break;
			}
		}
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMG_NPCController::TargetKey, nullptr);
		DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
	}

}
