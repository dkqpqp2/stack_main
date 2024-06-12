// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_PlayerProjectile.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AMG_PlayerProjectile::AMG_PlayerProjectile()
{
	SphereTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraEffectRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/Character/Niagara/PlayerAttack.PlayerAttack'"));
	if (NiagaraEffectRef.Object)
	{
		NiagaraProjectileEffect = NiagaraEffectRef.Object;
	}

	RootComponent = SphereTrigger;
	NiagaraComp->SetupAttachment(SphereTrigger);
	NiagaraComp->SetAsset(NiagaraProjectileEffect);
	NiagaraComp->SetRelativeScale3D(FVector(0.1, 0.1, 0.1));

	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 1000.f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
}

// Called when the game starts or when spawned
void AMG_PlayerProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}
