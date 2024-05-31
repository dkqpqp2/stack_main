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

protected:
	// 나중에 블프에서 변경하고 싶으면 UPROPERTY 붙여서 보이게 하자.
	float ItemScore = 1.f;

	UFUNCTION()
	void OnBoxComponentOverlapped(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult& SweepResult
	);
};
