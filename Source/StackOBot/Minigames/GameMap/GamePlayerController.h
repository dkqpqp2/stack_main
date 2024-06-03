// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API AGamePlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
protected:
	// 이변수 언젠가는 게임 인스턴스로 옮기긴 해야함...
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FString, TSubclassOf<APawn>> CharacterClassesMap;

public:
	const TSubclassOf<APawn>* const FindCharacterClass(const FString& CharacterClassName) const;
};
