// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LobbyGameStateBase.generated.h"


USTRUCT(BlueprintType)
struct FGameMapStruct
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MapURL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MapName;
};
/**
 * 
 */
UCLASS()
class STACKOBOT_API ALobbyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_MapIndex)
	int32 MapIndex = 0;

	void SetMapIndex(int32 NewMapIndex);


	UFUNCTION()
	void OnRep_MapIndex();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGameMapStruct> GameMaps;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
};
