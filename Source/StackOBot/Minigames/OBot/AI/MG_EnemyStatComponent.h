// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyData/EnemyStat.h"
#include "MG_EnemyStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/ );


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STACKOBOT_API UMG_EnemyStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMG_EnemyStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;

	void SetEnemyType(int32 InEnemyType);
	float GetCurrentEnemyType() const { return EnemyType; }
	FEnemyStat GetTotalStat() const { return EnemyStat; }
	float GetCurrentHp() { return CurrentHp; }
	float ApplyDamage(float InDamage);


protected:
	void SetHp(float NewHp);

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	int32 EnemyType;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	FEnemyStat EnemyStat;
};
