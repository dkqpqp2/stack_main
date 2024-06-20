// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class STACKOBOT_API AMG_EnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMG_EnemyBase();

	void Attack();
	FOnAttackEndDelegate OnAttackEnd;

	EMonsterType GetCurrentMonsterType() { return CurrentMonsterType; }

protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(VisibleAnywhere)
	EMonsterType CurrentMonsterType;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead();
	void PlayDeadAnimation();

public:
	virtual void Tick(float DeltaTime) override;
	
};
