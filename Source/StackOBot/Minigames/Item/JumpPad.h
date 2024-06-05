// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JumpPad.generated.h"

UCLASS()
class STACKOBOT_API AJumpPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJumpPad();

protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* JumpPadMesh;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* JumpPadCollision;

	UPROPERTY(EditAnywhere)
	class UArrowComponent* JumpDirectionArrow;

	UPROPERTY(EditAnywhere)
	float LaunchStrength;

	UFUNCTION()
	void OnOverlapJumpPad(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult& SweepResult
	);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
