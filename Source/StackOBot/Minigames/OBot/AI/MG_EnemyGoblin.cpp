// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_EnemyGoblin.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Minigames/OBot/AI/UI/MG_WidgetComponent.h"
#include "Minigames/OBot/AI/UI/MG_MonsterHpBar.h"

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

	CurrentMonsterType = EMonsterType::Goblin;
	MaxHp = 50.0f;
	CurrentHp = MaxHp;

}

void AMG_EnemyGoblin::BeginPlay()
{
	Super::BeginPlay();

	SetHp(MaxHp);
}

//float AMG_EnemyGoblin::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
//{
//	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
//
//	ApplyDamage(DamageAmount);
//
//	return DamageAmount;
//}

//void AMG_EnemyGoblin::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
//{
//	Super::SetAIAttackDelegate(InOnAttackFinished);
//
//	OnAttackFinished = InOnAttackFinished;
//}

void AMG_EnemyGoblin::AttackActionEnd(UAnimMontage* TargetMontage, bool InProperlyEnded)
{
	NotifyAttackActionEnd();
}

void AMG_EnemyGoblin::PlayAttackAnimation()
{
	Super::PlayAttackAnimation();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(AttackMontage, 1.0f);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMG_EnemyGoblin::AttackActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackMontage);
}

void AMG_EnemyGoblin::AttackByAI()
{
	Super::AttackByAI();

	PlayAttackAnimation();

}

void AMG_EnemyGoblin::NotifyAttackActionEnd()
{
	Super::NotifyAttackActionEnd();
	OnAttackFinished.ExecuteIfBound();
}

//float AMG_EnemyGoblin::ApplyDamage(float InDamage)
//{
//	Super::ApplyDamage(InDamage);
//	const float PrevHp = CurrentHp;
//	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);
//
//	SetHp(PrevHp - ActualDamage);
//	if (CurrentHp <= KINDA_SMALL_NUMBER)
//	{
//		OnHpZero.Broadcast();
//	}
//	return ActualDamage;
//}
//
//void AMG_EnemyGoblin::SetHp(float NewHp)
//{
//	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);
//	OnHpChanged.Broadcast(CurrentHp);
//}
//
//void AMG_EnemyGoblin::SetDead()
//{
//	Super::SetDead();
//
//	FTimerHandle DeadTimerHandle;
//	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
//		[&]()
//		{
//			Destroy();
//		}
//	), DeadEventDelayTime, false);
//
//}
//
//void AMG_EnemyGoblin::SetupCharacterWidget(UMG_UserWidget* InUserWidget)
//{
//	Super::SetupCharacterWidget(InUserWidget);
//
//	UMG_MonsterHpBar* HpBarWidget = Cast<UMG_MonsterHpBar>(InUserWidget);
//	if (HpBarWidget)
//	{
//		HpBarWidget->SetMaxHp(GetMaxHp());
//		HpBarWidget->UpdateHpBar(GetCurrentHp());
//		OnHpChanged.AddUObject(HpBarWidget, &UMG_MonsterHpBar::UpdateHpBar);
//	}
//}

