// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_EnemyBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Minigames/OBot/Player/MG_NPCController.h"
#include "GameFramework/CharacterMovementComponent.h"


AMG_EnemyBase::AMG_EnemyBase()
{
	
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AMG_NPCController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

}

void AMG_EnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}


void AMG_EnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AMG_EnemyBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!IsPlayerControlled())
	{
		CurrentMonsterType = EMonsterType::Goblin;
		GetCharacterMovement()->MaxWalkSpeed = 230.0f;
	}
}


