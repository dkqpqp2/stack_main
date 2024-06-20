// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "Minigames/OBot/Player/MG_NPCController.h"
#include "Minigames/OBot/Character/MG_ShootingCharacterPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "Minigames/OBot/AI/MG_EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	float DetectRadius = 600.0f;

	if (nullptr == World)
	{
		return;
	}
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


	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMG_NPCController::TargetKey, Pawn);

				AMG_EnemyBase* Enemy = Cast<AMG_EnemyBase>(ControllingPawn);
				switch (Enemy->GetCurrentMonsterType())
				{
					case EMonsterType::Goblin:
						Enemy->GetCharacterMovement()->MaxWalkSpeed = 450.f;
						break;
					case EMonsterType::Ghoul:
						Enemy->GetCharacterMovement()->MaxWalkSpeed = 450.f;
						break;
					case EMonsterType::Skeleton:
						Enemy->GetCharacterMovement()->MaxWalkSpeed = 450.f;
						break;
					case EMonsterType::Lich:
						Enemy->GetCharacterMovement()->MaxWalkSpeed = 450.f;
						break;
					default:
						break;
				}
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Blue, false, 0.2f);
				return;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMG_NPCController::TargetKey, nullptr);
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}
