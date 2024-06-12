// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pla.generated.h"

UCLASS()
class STACKOBOT_API APla : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APla();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float MovedDistance = 100;

	UPROPERTY(EditAnywhere)
	float DistanceMoved = -1;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "Move")
	FVector PlatformVelocity = FVector(100, 0, 0);

	UPROPERTY(EditAnywhere, Category ="Rotate")
	FRotator PlatformWheel;

	UPROPERTY(VisibleAnywhere)
	FVector StartLocation;

	void MovePlatform(float Deltatime);
	void RotatePlatform(float Deltatime);

	bool ShouldMoved() const;
	float GetDistanceMoved() const;


};
