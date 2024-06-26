// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Minigames/OBot/Interface/MG_AIInterface.h"
#include "Minigames/OBot/Interface/MG_CharacterWidgetInterface.h"
#include "Minigames/OBot/Interface/MG_AnimationAttackInterface.h"
#include "MG_EnemyBase.generated.h"

UENUM()
enum class EMonsterType
{
	None,
	Goblin,
	Ghoul,
	Skeleton,
	Zombie,
	Lich
};

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);

UCLASS()
class STACKOBOT_API AMG_EnemyBase : public ACharacter, public IMG_AIInterface, public IMG_CharacterWidgetInterface, public IMG_AnimationAttackInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMG_EnemyBase();

	EMonsterType GetCurrentMonsterType() { return CurrentMonsterType; }

	virtual void PostInitializeComponents() override;

	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;

	float GetMaxHp() { return MaxHp; }
	float GetCurrentHp() { return CurrentHp; }
	float ApplyDamage(float InDamage);
	virtual void AttackHitCheck();

protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(VisibleAnywhere)
	EMonsterType CurrentMonsterType;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dead, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack)
	TObjectPtr<class UMG_EnemyComboActionData> ComboActionData;

	virtual void SetDead();

	UFUNCTION(NetMulticast, Unreliable)
	virtual void PlayDeadAnimation();
	virtual void PlayAttackAnimation();
	virtual void ProcessComboAttack();
	virtual void ComboActionBegin();
	virtual void AttackActionEnd(class UAnimMontage* TargetMontage, bool InProperlyEnded);
	virtual void NotifyAttackActionEnd();
	void SetComboCheckTimer();
	void ComboCheck();


	float DeadEventDelayTime = 1.75f;
	int32 CurrentCombo = 0;
	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat)
	TObjectPtr<class UMG_EnemyStatComponent> Stat;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget)
	TObjectPtr<class UMG_WidgetComponent> HpBar;

	virtual void SetupCharacterWidget(class UMG_UserWidget* InUserWidget);

protected:
	void SetHp(float NewHp);

	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float MaxHp;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentHP, Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float AttackDamage;

protected:
	virtual void ReadyForReplication();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_CurrentHp();

public:
	TObjectPtr<class AMG_EnemySpawnPoint> SpawnPoint;

	void SetItemSpawnPoint(class AMG_EnemySpawnPoint* Point)
	{
		SpawnPoint = Point;
	}
};
