// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MG_LichPoison.generated.h"

UCLASS()
class STACKOBOT_API AMG_LichPoison : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMG_LichPoison();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> PoisonBox;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> PoisonActor;
	
	float PoisonDamage = 1.0f;
	FTimerHandle PoisonTimer;
	void OverlapBeginPoison(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void OverlapEndPoison(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ApplyPoisonDamage();

};
