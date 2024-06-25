#include "MG_EnemyStatComponent.h"


UMG_EnemyStatComponent::UMG_EnemyStatComponent()
{
	MaxHp = 50.0f;
}

void UMG_EnemyStatComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHp(MaxHp);
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
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}
	OnHpChanged.Broadcast(CurrentHp);
}


