// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnemyStat.h"
#include "MG_GameSingleton.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LogUMG_GameSingleton, Error, All);
/**
 * 
 */
UCLASS()
class STACKOBOT_API UMG_GameSingleton : public UObject
{
	GENERATED_BODY()
public:
	UMG_GameSingleton();
	static UMG_GameSingleton& Get();

public:
	FEnemyStat GetEnemyStat(int32 Index) const { return EnemyStatTable.IsValidIndex(Index - 1) ? EnemyStatTable[Index - 1] : FEnemyStat(); }

	UPROPERTY()
	int32 EnemyTypeNum;

private:
	TArray<FEnemyStat> EnemyStatTable;
	
};
