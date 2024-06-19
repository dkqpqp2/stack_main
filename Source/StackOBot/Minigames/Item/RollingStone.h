// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "RollingStone.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChangeFaceDelegate, AActor*, OtherActor, float, indexNumber);

UCLASS()
class STACKOBOT_API ARollingStone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARollingStone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StoneMesh;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* StoneCollision;

	UPROPERTY(Replicated, EditAnywhere)
	FVector Location;

	UPROPERTY(Replicated, EditAnywhere)
	FRotator Rotation;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Character Settings")
	float BounceForce = 1000.0f;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


	UPROPERTY(BlueprintAssignable)
	FChangeFaceDelegate OnChangeFace;

	UFUNCTION(BlueprintCallable)
	void ChangeFace(AActor* OtherActor, float indexnumber);

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty>& OutLifetimeProps) const override;

};
