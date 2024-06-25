#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnemyStat.generated.h"

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

USTRUCT(BlueprintType)
struct FEnemyStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FEnemyStat() : MaxHp(0.0f), Attack(0.0f), AttackRange(0.0f), AttackSpeed(0.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	EMonsterType EnemyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MovementSpeed;
};