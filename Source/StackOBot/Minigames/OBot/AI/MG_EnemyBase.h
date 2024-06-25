// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Minigames/OBot/Interface/MG_AIInterface.h"
#include "Minigames/OBot/Interface/MG_CharacterWidgetInterface.h"
#include "EnemyData/EnemyStat.h"
#include "MG_EnemyBase.generated.h"

UCLASS()
class STACKOBOT_API AMG_EnemyBase : public ACharacter, public IMG_AIInterface, public IMG_CharacterWidgetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMG_EnemyBase();

	EMonsterType GetCurrentMonsterType() { return CurrentMonsterType; }

	virtual void PostInitializeComponents() override;

protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	UPROPERTY(VisibleAnywhere, Category = EnemyType)
	EMonsterType CurrentMonsterType;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Dead, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> AttackMontage;

	
	virtual void SetDead();
	UFUNCTION(NetMulticast, Unreliable)
	virtual void PlayDeadAnimation();
	virtual void PlayAttackAnimation();
	virtual void AttackActionEnd(class UAnimMontage* TargetMontage, bool InProperlyEnded);
	virtual void NotifyAttackActionEnd();

	float DeadEventDelayTime = 2.0f;

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
	
};
