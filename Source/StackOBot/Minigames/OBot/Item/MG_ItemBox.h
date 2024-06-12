// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MG_ItemBox.generated.h"

UCLASS()
class STACKOBOT_API AMG_ItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMG_ItemBox();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = Trigger)
	TObjectPtr<class USphereComponent> Trigger;

	UPROPERTY(VisibleAnywhere, Category = Mesh)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, Category = Niagara)
	TObjectPtr<class UNiagaraComponent> Effect;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_ItemBoxEffect();

	UFUNCTION()
	void OnEffectFinished(class UNiagaraComponent* NiagaraSystem);

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing = OnRep_ServerRotationYaw)
	float ServerRotationYaw;

	UPROPERTY(ReplicatedUsing = OnRep_ServerRotationRoll)
	float ServerRotationRoll;

	UFUNCTION()
	void OnRep_ServerRotationYaw();

	UFUNCTION()
	void OnRep_ServerRotationRoll();

	float RotationRate = 30.0f;

	TObjectPtr<class AMG_ItemSpawnPoint> SpawnPoint;

	void SetItemSpawnPoint(class AMG_ItemSpawnPoint* Point)
	{
		SpawnPoint = Point;
	}

protected:
	void GetNewItemTo(AActor* OtherActor);


};
