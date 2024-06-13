// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Podium.generated.h"

class UCameraComponent;

UCLASS()
class STACKOBOT_API APodium : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APodium();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> PodiumMesh;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> PlayerRank1Location;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> PlayerRank2Location;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> PlayerRank3Location;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> PlayerRank4Location;

public:
	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<USceneComponent>> PlayerLocations;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCameraComponent> PodiumCamera;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
