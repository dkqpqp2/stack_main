// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_GameSingleton.h"

DEFINE_LOG_CATEGORY(LogUMG_GameSingleton);

UMG_GameSingleton::UMG_GameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Script/Engine.DataTable'/Game/Character/AI/DT_EnemyStat.DT_EnemyStat'"));
	if (nullptr != DataTableRef.Object)
	{
		const UDataTable* DataTable = DataTableRef.Object;

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, EnemyStatTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FEnemyStat*>(Value);
			}
		);
	}

	EnemyTypeNum = EnemyStatTable.Num();
	ensure(EnemyTypeNum > 0);

}

UMG_GameSingleton& UMG_GameSingleton::Get()
{
	UMG_GameSingleton* Singleton = CastChecked<UMG_GameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LogUMG_GameSingleton, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<UMG_GameSingleton>();
}
