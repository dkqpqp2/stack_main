// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_NPCController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "StackOBot.h"


const FName AMG_NPCController::HomePosKey(TEXT("HomePos"));
const FName AMG_NPCController::PatrolPosKey(TEXT("PatrolPos"));
const FName AMG_NPCController::TargetKey(TEXT("Target"));

AMG_NPCController::AMG_NPCController()
{
	//RepeatInterval = 3.0f;

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObjectRef(TEXT("/Script/AIModule.BlackboardData'/Game/Character/AI/BB_NPCCharacter.BB_NPCCharacter'"));
	if (BBObjectRef.Object)
	{
		BBAsset = BBObjectRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObjectRef(TEXT("/Script/AIModule.BehaviorTree'/Game/Character/AI/BT_NPCCharacter.BT_NPCCharacter'"));
	if (BTObjectRef.Object)
	{
		BTAsset = BTObjectRef.Object;
	}
}

void AMG_NPCController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UBlackboardComponent* BlackboardComp = Blackboard;
	if (UseBlackboard(BBAsset, BlackboardComp))
	{
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogTemp, Warning, TEXT("AIController couldn't run behavior tree!"));
		}
	}

	//GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &AMG_NPCController::OnRepeatTimer, RepeatInterval, true);
}

//void AMG_NPCController::OnUnPossess()
//{
//	Super::OnUnPossess();
//	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
//}
//
//void AMG_NPCController::OnRepeatTimer()
//{
//	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
//	if (nullptr == NavSystem)
//	{
//		return;
//	}
//
//	FNavLocation NextLocation;
//	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextLocation))
//	{
//		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
//	}
//}
