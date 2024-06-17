// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MG_MissileTest.generated.h"

UCLASS()
class STACKOBOT_API AMG_MissileTest : public AActor
{
	GENERATED_BODY()
private:

	bool HasTarget;
	bool HasNoTarget;
	TObjectPtr<class AActor> Target;

	float DelayTimer;
	bool HasFinishedDelay;

	float LifeTimeCountDown;
	bool CanBeDestroyed;
	void Explode();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlapComp, AActor* OtherActro, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult);

	
public:	
	// Sets default values for this actor's properties
	AMG_MissileTest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<class AMG_CharacterBase> PlayerInWorld;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DelayLogic(float DeltaTime);

	void FindPlayer();
	void UpdateTarget();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
	TObjectPtr<class UBoxComponent> CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	TObjectPtr<class UStaticMeshComponent> MissileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(EditAnywhere, Category = FX)
	TObjectPtr<class UNiagaraSystem> ExplosionSystem;

	//class UNiagaraSystemComponent* PlayExplosion(class UNiagaraSystem* Explosion);
	TObjectPtr<class UNiagaraComponent> PlayExplosion(class UNiagaraSystem* Explosion);

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	TObjectPtr<class USoundCue> ExplosionSound;

	TObjectPtr<class UAudioComponent> PlayExplosionSound(class USoundCue* Sound);







};
