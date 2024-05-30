// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine.h"
#include "ItemActor.generated.h"

UCLASS()
class STACKOBOT_API AItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemActor();
protected:
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* mCollision;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* mBox;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
