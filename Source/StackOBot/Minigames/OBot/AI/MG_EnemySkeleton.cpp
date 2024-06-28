// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_EnemySkeleton.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Minigames/OBot/AI/UI/MG_WidgetComponent.h"
#include "Minigames/OBot/AI/UI/MG_MonsterHpBar.h"
#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"

AMG_EnemySkeleton::AMG_EnemySkeleton()
{
	GetCapsuleComponent()->InitCapsuleSize(30.0f, 80.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));

	GetCharacterMovement()->JumpZVelocity = 700.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->SetWalkableFloorAngle(50.0f);
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
	GetCharacterMovement()->GravityScale = 1.0f;
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0, 0.0, -80.0), FRotator(0.0, -90.0, 0.0));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("EnemyMesh"));

	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword"));
	SwordMesh->SetupAttachment(GetMesh(), TEXT("FX"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletonMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/UndeadPack/SkeletonEnemy/Mesh/SK_Skeleton.SK_Skeleton'"));
	if (SkeletonMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(SkeletonMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> SkeletonAnimInstanceClassRef(TEXT("/Game/Character/AI/Skeleton/MG_Skeleton.MG_Skeleton_C"));
	if (SkeletonAnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(SkeletonAnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SwordMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/UndeadPack/SkeletonEnemy/Mesh/Weapon/Sword/SM_Sword.SM_Sword'"));
	if (SwordMeshRef.Object)
	{
		SwordMesh->SetStaticMesh(SwordMeshRef.Object);
	}

	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));

	CurrentMonsterType = EMonsterType::Skeleton;
	MaxHp = 250.0f;
	AttackDamage = 23.0f;
	IsEquipped = false;
	DeadEventDelayTime = 1.0f;
}

void AMG_EnemySkeleton::BeginPlay()
{
	Super::BeginPlay();

	CurrentHp = MaxHp;
}

void AMG_EnemySkeleton::PlayEquipSwordMontage_Implementation()
{
	AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(EquipSwordMontage, 1.0f);
	IsEquipped = true;
}

void AMG_EnemySkeleton::SetDead()
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

bool AMG_EnemySkeleton::GetIsEquipped()
{
	return IsEquipped;
}

void AMG_EnemySkeleton::AttackByAI_Implementation()
{
	PlayAttackAnimation();
}

void AMG_EnemySkeleton::NotifyAttackActionEnd_Implementation()
{
	Super::NotifyAttackActionEnd();
	OnAttackFinished.ExecuteIfBound();
}
