// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MG_EnemyComboActionData.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API UMG_EnemyComboActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UMG_EnemyComboActionData();

	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionNamePrefix;

	UPROPERTY(EditAnywhere, Category = Name)
	uint8 MaxComboCount;

	UPROPERTY(EditAnywhere, Category = Name)
	float FrameRate;

	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> EffectiveFrameCount;
};
