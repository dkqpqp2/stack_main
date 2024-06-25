#include "MG_EnemyStatComponent.h"
#include "EnemyData/MG_GameSingleton.h"


UMG_EnemyStatComponent::UMG_EnemyStatComponent()
{
}

void UMG_EnemyStatComponent::BeginPlay()
{
	Super::BeginPlay();

	SetEnemyType(EnemyType);
	SetHp(EnemyStat.MaxHp);
}

void UMG_EnemyStatComponent::SetEnemyType(int32 InEnemyType)
{
	EnemyType = FMath::Clamp(InEnemyType, 1, UMG_GameSingleton::Get().EnemyTypeNum);
	EnemyStat = UMG_GameSingleton::Get().GetEnemyStat(EnemyType);
	
}

float UMG_EnemyStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);
	
	return ActualDamage;
}

void UMG_EnemyStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, EnemyStat.MaxHp);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}
	OnHpChanged.Broadcast(CurrentHp);
}


