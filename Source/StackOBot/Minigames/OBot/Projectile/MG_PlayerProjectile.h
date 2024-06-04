// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MG_PlayerProjectile.generated.h"

UCLASS()
class STACKOBOT_API AMG_PlayerProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMG_PlayerProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Trigger)
	TObjectPtr<class USphereComponent> SphereTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Niagara)
	TObjectPtr<class UNiagaraComponent> NiagaraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Niagara)
	TObjectPtr<class UNiagaraSystem> NiagaraProjectileEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Trigger)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;
};
