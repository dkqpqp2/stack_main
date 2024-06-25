// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_EnemyGoblin.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Minigames/OBot/AI/UI/MG_WidgetComponent.h"
#include "Minigames/OBot/AI/UI/MG_MonsterHpBar.h"
#include "MG_EnemyStatComponent.h"
#include "EnemyData/EnemyStat.h"

AMG_EnemyGoblin::AMG_EnemyGoblin()
{
	GetCapsuleComponent()->InitCapsuleSize(60.0f, 60.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));

	GetCharacterMovement()->JumpZVelocity = 700.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->SetWalkableFloorAngle(50.0f);
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
	GetCharacterMovement()->GravityScale = 1.0f;
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0, 0.0, -60.0), FRotator(0.0, -90.0,0.0));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("EnemyMesh"));


	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GoblinMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/UndeadPack/EnemyGoblin/Mesh/SM_EnemyGoblin.SM_EnemyGoblin'"));
	if (GoblinMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(GoblinMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> GoblinAnimInstanceClassRef(TEXT("/Game/Character/AI/Goblin/MG_Goblin.MG_Goblin_C"));
	if (GoblinAnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(GoblinAnimInstanceClassRef.Class);
	}

	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));
	
	CurrentMonsterType = EMonsterType::Ghoul;
}

void AMG_EnemyGoblin::BeginPlay()
{
	Super::BeginPlay();
}

void AMG_EnemyGoblin::SetDead()
{
	Super::SetDead();

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);
}

void AMG_EnemyGoblin::AttackByAI_Implementation()
{
	PlayAttackAnimation();
}

void AMG_EnemyGoblin::NotifyAttackActionEnd_Implementation()
{
	Super::NotifyAttackActionEnd();
	OnAttackFinished.ExecuteIfBound();
}


