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

	CurrentMonsterType = EMonsterType::None;
}

void AMG_EnemyBase::Attack()
{
}


void AMG_EnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

float AMG_EnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	SetDead();

	return DamageAmount;
}


void AMG_EnemyBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(false);
}

void AMG_EnemyBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DeadMontage, 1.0f);

}

void AMG_EnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}




