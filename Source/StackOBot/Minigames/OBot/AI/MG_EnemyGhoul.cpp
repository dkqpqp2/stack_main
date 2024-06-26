// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_EnemyGhoul.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Minigames/OBot/AI/UI/MG_WidgetComponent.h"
#include "Minigames/OBot/AI/UI/MG_MonsterHpBar.h"

AMG_EnemyGhoul::AMG_EnemyGhoul()
{
	GetCapsuleComponent()->InitCapsuleSize(55.0f, 70.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));

	GetCharacterMovement()->JumpZVelocity = 700.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->SetWalkableFloorAngle(50.0f);
	GetCharacterMovement()->MaxWalkSpeed = 230.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
	GetCharacterMovement()->GravityScale = 1.0f;
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0, 0.0, -60.0), FRotator(0.0, -90.0, 0.0));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("EnemyMesh"));


	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GhoulMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/UndeadPack/Ghoul/Mesh/SK_Ghoul_Full.SK_Ghoul_Full'"));
	if (GhoulMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(GhoulMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> GhoulAnimInstanceClassRef(TEXT("/Game/Character/AI/Ghoul/MG_Ghoul.MG_Ghoul_C"));
	if (GhoulAnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(GhoulAnimInstanceClassRef.Class);
	}

	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));

	CurrentMonsterType = EMonsterType::Ghoul;
	MaxHp = 150.0f;
	AttackDamage = 10.0f;
}

void AMG_EnemyGhoul::BeginPlay()
{
	Super::BeginPlay();

	CurrentHp = MaxHp;
}

void AMG_EnemyGhoul::SetDead()
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

void AMG_EnemyGhoul::AttackByAI_Implementation()
{
	PlayAttackAnimation();
}

void AMG_EnemyGhoul::NotifyAttackActionEnd_Implementation()
{
	Super::NotifyAttackActionEnd();
	OnAttackFinished.ExecuteIfBound();
}
