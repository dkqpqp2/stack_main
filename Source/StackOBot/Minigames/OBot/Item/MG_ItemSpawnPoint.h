// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MG_ItemSpawnPoint.generated.h"

UCLASS()
class STACKOBOT_API AMG_ItemSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMG_ItemSpawnPoint();

protected:
	TObjectPtr<class USceneComponent> Root;

#if WITH_EDITORONLY_DATA
	TObjectPtr<class UArrowComponent> Arrow;
#endif

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AMG_ItemBox> SpawnClass;

	TObjectPtr<class AMG_ItemBox> SpawnBox;

	UPROPERTY(EditAnywhere)
	float SpawnTime;

	float AccTime;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void Spawn();

public:
	void ClearSpawnObject();


};
