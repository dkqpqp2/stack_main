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
#include "Minigames/OBot/AI/DataAsset/MG_EnemyComboActionData.h"
#include "Net/UnrealNetwork.h"
#include "Minigames/OBot/AI/SpawnPoint/MG_EnemySpawnPoint.h"

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
	SetReplicates(true);
}


void AMG_EnemyBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	OnHpZero.AddUObject(this, &AMG_EnemyBase::SetDead);
}

float AMG_EnemyBase::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);

	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

float AMG_EnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	ApplyDamage(DamageAmount);

	
	//SetDead();

	return DamageAmount;
}


void AMG_EnemyBase::SetDead()
{
	if (HasAuthority())
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		PlayDeadAnimation();
		SetActorEnableCollision(false);
		Cast<AMG_NPCController>(GetController())->StopAI();

		if (SpawnPoint)
		{
			SpawnPoint->ClearSpawnObject();
		}
	}

}

void AMG_EnemyBase::PlayDeadAnimation_Implementation()
{
	AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DeadMontage, 1.0f);
	HpBar->SetHiddenInGame(true);
}

void AMG_EnemyBase::PlayAttackAnimation()
{
	AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(AttackMontage, 1.0f);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMG_EnemyBase::AttackActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackMontage);
}


void AMG_EnemyBase::ProcessComboAttack()
{
	if (CurrentCombo == 0)
	{
		ComboActionBegin();
		return;
	}

	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboCommand = false;
	}
	else
	{
		HasNextComboCommand = true;
	}
	
}

void AMG_EnemyBase::ComboActionBegin()
{
	CurrentCombo = 1;

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	const float AttackSpeedRate = 1.0f;
	AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AttackMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMG_EnemyBase::AttackActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackMontage);

	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void AMG_EnemyBase::AttackActionEnd(UAnimMontage* TargetMontage, bool InProperlyEnded)
{
	/*ensure(CurrentCombo != 0);

	CurrentCombo = 0;

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);*/

	NotifyAttackActionEnd();
}

void AMG_EnemyBase::NotifyAttackActionEnd()
{
}

void AMG_EnemyBase::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float AttackSpeedRate = 2.0f;
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AMG_EnemyBase::ComboCheck, ComboEffectiveTime, false);
	}
}

void AMG_EnemyBase::ComboCheck()
{
	ComboTimerHandle.Invalidate();
	if (HasNextComboCommand)
	{
		AnimInstance = GetMesh()->GetAnimInstance();

		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
		AnimInstance->Montage_JumpToSection(NextSection, AttackMontage);
		SetComboCheckTimer();
		HasNextComboCommand = false;
	}
}

void AMG_EnemyBase::AttackHitCheck()
{
	if (HasAuthority())
	{
		FHitResult HitResult;
		FCollisionQueryParams Params(SCENE_QUERY_STAT(AttackByAI), true, this);

		
		FVector SocketLocation;
		if (GetMesh()->GetSocketByName(TEXT("LHand_Socket")) == nullptr)
		{
			SocketLocation = GetMesh()->GetSocketLocation(TEXT("RHand_Socket"));
		}
		else
		{
			SocketLocation = GetMesh()->GetSocketLocation(TEXT("LHand_Socket"));
		}
		const FVector Start = SocketLocation + GetActorForwardVector();
		const FVector End = Start + GetActorForwardVector() * AttackRange;

		bool HitDetected = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(AttackRadius), Params);

		if (HitDetected)
		{
			FDamageEvent DamageEvent;
			HitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
		}

#if ENABLE_DRAW_DEBUG

		FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
		float CapsuleHalfHeight = AttackRange * 0.5f;
		FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

		DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);

#endif
	}
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
	return DetectRange;
}

float AMG_EnemyBase::GetAIAttackRange()
{
	return 150.0f;
}

float AMG_EnemyBase::GetAITurnSpeed()
{
	return 2.0f;
}

void AMG_EnemyBase::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	if (!HasNextComboCommand)
	{
		OnAttackFinished = InOnAttackFinished;
	}
	
}

void AMG_EnemyBase::AttackByAI()
{
	//ProcessComboAttack();
}

void AMG_EnemyBase::SetupCharacterWidget(UMG_UserWidget* InUserWidget)
{
	UMG_MonsterHpBar* HpBarWidget = Cast<UMG_MonsterHpBar>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->SetMaxHp(GetMaxHp());
		HpBarWidget->UpdateHpBar(GetCurrentHp());
		OnHpChanged.AddUObject(HpBarWidget, &UMG_MonsterHpBar::UpdateHpBar);
	}
}

void AMG_EnemyBase::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);
	
	OnHpChanged.Broadcast(CurrentHp);
}

void AMG_EnemyBase::ReadyForReplication()
{

}

void AMG_EnemyBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMG_EnemyBase, CurrentHp);
}

void AMG_EnemyBase::OnRep_CurrentHp()
{
	OnHpChanged.Broadcast(CurrentHp);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}
}




