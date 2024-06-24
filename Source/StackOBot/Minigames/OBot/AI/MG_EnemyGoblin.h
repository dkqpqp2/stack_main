// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MG_EnemyBase.h"
#include "MG_EnemyGoblin.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API AMG_EnemyGoblin : public AMG_EnemyBase
{
	GENERATED_BODY()

public:
	AMG_EnemyGoblin();

protected:
	virtual void BeginPlay() override;
	//virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	//virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AttackActionEnd(class UAnimMontage* TargetMontage, bool InProperlyEnded) override;
	virtual void PlayAttackAnimation() override;
	virtual void AttackByAI() override;
	virtual void NotifyAttackActionEnd() override;

public:
	virtual float GetMaxHp() { return MaxHp; }
	virtual float GetCurrentHp() { return CurrentHp; }
	//virtual float ApplyDamage(float InDamage) override;

protected:
	//virtual void SetHp(float NewHp) override;
	//void SetDead() override;
	//virtual void SetupCharacterWidget(class UMG_UserWidget* InUserWidget) override;
	
};
