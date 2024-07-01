// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_EnemyBowSkeleton.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Minigames/OBot/AI/UI/MG_WidgetComponent.h"
#include "Minigames/OBot/AI/UI/MG_MonsterHpBar.h"
#include "Components/SkeletalMeshComponent.h"

AMG_EnemyBowSkeleton::AMG_EnemyBowSkeleton()
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

	BowMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bow"));
	BowMesh->SetupAttachment(GetMesh(), TEXT("SKT_Bow"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletonMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/UndeadPack/SkeletonEnemy/Mesh/SK_Skeleton.SK_Skeleton'"));
	if (SkeletonMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(SkeletonMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BowMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/UndeadPack/SkeletonEnemy/Mesh/Weapon/Bow/SK_Bow.SK_Bow'"));
	if (BowMeshRef.Object)
	{
		BowMesh->SetSkeletalMesh(BowMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> BowSkeletonAnimInstanceClassRef(TEXT("/Game/Character/AI/BowSkeleton/MG_BowSkeleton.MG_BowSkeleton_C"));
	if (BowSkeletonAnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(BowSkeletonAnimInstanceClassRef.Class);
	}

	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 160.0f));

	CurrentMonsterType = EMonsterType::Skeleton;
	MaxHp = 150.0f;
	AttackDamage = 5.0f;
	DeadEventDelayTime = 1.0f;

}

void AMG_EnemyBowSkeleton::BeginPlay()
{
	Super::BeginPlay();

	CurrentHp = MaxHp;
}

void AMG_EnemyBowSkeleton::FindTarget()
{
}

void AMG_EnemyBowSkeleton::SetDead()
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

void AMG_EnemyBowSkeleton::AttackByAI_Implementation()
{
	PlayAttackAnimation();
}

void AMG_EnemyBowSkeleton::NotifyAttackActionEnd_Implementation()
{
	Super::NotifyAttackActionEnd();
	OnAttackFinished.ExecuteIfBound();
}
