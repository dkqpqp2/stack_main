// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_SkeletonArrow.h"
#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AMG_SkeletonArrow::AMG_SkeletonArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMesh"));


	RootComponent = BoxCollision;

	ArrowMesh->SetupAttachment(BoxCollision);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ArrowMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/UndeadPack/SkeletonEnemy/Mesh/Weapon/Bow/Arrow/SM_Arrow.SM_Arrow'"));
	if (nullptr != ArrowMeshRef.Object)
	{
		ArrowMesh->SetStaticMesh(ArrowMeshRef.Object);
	}

	ArrowMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ArrowMovement"));
	ArrowMovement->UpdatedComponent = BoxCollision;
	ArrowMovement->InitialSpeed = 500.0f;
	ArrowMovement->MaxSpeed = 1000.0f;
	ArrowMovement->ProjectileGravityScale = 0.5f;
	ArrowMovement->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AMG_SkeletonArrow::BeginPlay()
{
	Super::BeginPlay();
	
}

float AMG_SkeletonArrow::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return DamageAmount;
}

// Called every frame
void AMG_SkeletonArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMG_SkeletonArrow::LaunchProjectile()
{
	ArrowMovement->Activate();
}

