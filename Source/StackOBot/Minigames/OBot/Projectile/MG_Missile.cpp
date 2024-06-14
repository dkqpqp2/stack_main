// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_Missile.h"
#include "Engine.h"

// Sets default values
AMG_Missile::AMG_Missile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	RootComponent = ProjectileMesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 800.0f;
	ProjectileMovement->MaxSpeed = 1000.0f;


}

// Called when the game starts or when spawned
void AMG_Missile::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileMovement->Velocity = GetActorUpVector() * 300.f;
}

// Called every frame
void AMG_Missile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

