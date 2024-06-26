// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsInAttackRange.h"
#include "Minigames/OBot/Player/MG_NPCController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Minigames/OBot/Interface/MG_AIInterface.h"


UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (nullptr == ControllingPawn)
	{
		return false;
	}

	IMG_AIInterface* AIPawn = Cast<IMG_AIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return false;
	}

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMG_NPCController::TargetKey));
	if (nullptr == Target)
	{
		return false;
	}

	float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);
	float AttackRangeWithRadius = AIPawn->GetAIAttackRange();
	bResult = (DistanceToTarget <= AttackRangeWithRadius);

	return bResult;
}
