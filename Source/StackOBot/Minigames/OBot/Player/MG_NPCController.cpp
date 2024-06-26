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
	//test
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObjectRef(TEXT("/Script/AIModule.BlackboardData'/Game/Character/AI/BB_NPCCharacter.BB_NPCCharacter'"));
	if (nullptr != BBObjectRef.Object)
	{
		BBAsset = BBObjectRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObjectRef(TEXT("/Script/AIModule.BehaviorTree'/Game/Character/AI/BT_NPCCharacter.BT_NPCCharacter'"));
	if (nullptr != BTObjectRef.Object)
	{
		BTAsset = BTObjectRef.Object;
	}
}

void AMG_NPCController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}

void AMG_NPCController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());

		bool Result = RunBehaviorTree(BTAsset);
		
	}
}

void AMG_NPCController::StopAI()
{
	BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

