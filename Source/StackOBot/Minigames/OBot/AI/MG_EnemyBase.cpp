// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_EnemyBase.h"
#include "MG_EnemyGoblin.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Minigames/OBot/Player/MG_NPCController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Minigames/OBot/AI/UI/MG_WidgetComponent.h"
#include "Animation/AnimMontage.h"
#include "Minigames/OBot/AI/UI/MG_MonsterHpBar.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "MG_EnemyStatComponent.h"

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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Character/AI/Goblin/MG_GoblinDead.MG_GoblinDead'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}
	Stat = CreateDefaultSubobject<UMG_EnemyStatComponent>(TEXT("Stat"));

	HpBar = CreateDefaultSubobject<UMG_WidgetComponent>(TEXT("HpBar"));
	HpBar->SetupAttachment(GetMesh());
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/Character/AI/Goblin/MG_EnemyHpBar.MG_EnemyHpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(150.0f, 15.0f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	CurrentMonsterType = EMonsterType::None;
}


void AMG_EnemyBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->OnHpZero.AddUObject(this, &AMG_EnemyBase::SetDead);
}

float AMG_EnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Stat->ApplyDamage(DamageAmount);
	//SetDead();

	return DamageAmount;
}


void AMG_EnemyBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(false);
	HpBar->SetHiddenInGame(true);
	Cast<AMG_NPCController>(GetController())->StopAI();
}

void AMG_EnemyBase::PlayDeadAnimation_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DeadMontage, 1.0f);
}

void AMG_EnemyBase::PlayAttackAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(AttackMontage, 1.0f);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMG_EnemyBase::AttackActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackMontage);
}

void AMG_EnemyBase::AttackActionEnd(UAnimMontage* TargetMontage, bool InProperlyEnded)
{
	NotifyAttackActionEnd();
}

void AMG_EnemyBase::NotifyAttackActionEnd()
{
}

void AMG_EnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

float AMG_EnemyBase::GetAIPatrolRadius()
{
	return 500.0f;
}

float AMG_EnemyBase::GetAIDetectRange()
{
	return 600.0f;
}

float AMG_EnemyBase::GetAIAttackRange()
{
	return 250.0f;
}

float AMG_EnemyBase::GetAITurnSpeed()
{
	return 2.0f;
}

void AMG_EnemyBase::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AMG_EnemyBase::AttackByAI()
{
}

void AMG_EnemyBase::SetupCharacterWidget(UMG_UserWidget* InUserWidget)
{
	UMG_MonsterHpBar* HpBarWidget = Cast<UMG_MonsterHpBar>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->SetMaxHp(Stat->GetTotalStat().MaxHp);
		HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UMG_MonsterHpBar::UpdateHpBar);
	}
}




