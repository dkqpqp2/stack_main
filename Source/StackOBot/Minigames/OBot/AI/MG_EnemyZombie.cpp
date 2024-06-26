// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_EnemyZombie.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Minigames/OBot/AI/UI/MG_WidgetComponent.h"
#include "Minigames/OBot/AI/UI/MG_MonsterHpBar.h"

AMG_EnemyZombie::AMG_EnemyZombie()
{
	GetCapsuleComponent()->InitCapsuleSize(80.0f, 130.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));
	GetCapsuleComponent()->SetRelativeScale3D(FVector(0.5, 0.5, 0.5));

	GetCharacterMovement()->JumpZVelocity = 700.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->SetWalkableFloorAngle(50.0f);
	GetCharacterMovement()->MaxWalkSpeed = 130.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
	GetCharacterMovement()->GravityScale = 1.0f;
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0, 0.0, -130.0), FRotator(0.0, -90.0, 0.0));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("EnemyMesh"));


	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ZombieMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/UndeadPack/Zombie/Mesh/SK_Zombie.SK_Zombie'"));
	if (ZombieMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(ZombieMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> ZombieAnimInstanceClassRef(TEXT("/Game/Character/AI/Zombie/MG_Zombie.MG_Zombie_C"));
	if (ZombieAnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(ZombieAnimInstanceClassRef.Class);
	}

	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));

	CurrentMonsterType = EMonsterType::Zombie;
	MaxHp = 444.0f;
	AttackDamage = 4.0f;
}

void AMG_EnemyZombie::BeginPlay()
{
	Super::BeginPlay();

	CurrentHp = MaxHp;
}

void AMG_EnemyZombie::SetDead()
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

void AMG_EnemyZombie::AttackByAI_Implementation()
{
	PlayAttackAnimation();
}

void AMG_EnemyZombie::NotifyAttackActionEnd_Implementation()
{
	Super::NotifyAttackActionEnd();
	OnAttackFinished.ExecuteIfBound();
}
