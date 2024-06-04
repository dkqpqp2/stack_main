// Fill out your copyright notice in the Description page of Project Settings.


#include "Minigames/Item/Pla.h"

// Sets default values
APla::APla()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APla::BeginPlay()
{
	Super::BeginPlay();
	
	StartLocation = GetActorLocation();
}

// Called every frame
void APla::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MovePlatform(DeltaTime);
	RotatePlatform(DeltaTime);

}

void APla::RotatePlatform(float Deltatime)
{
	FRotator CurrentRotation = GetActorRotation();
	CurrentRotation = CurrentRotation + (PlatformWheel * Deltatime);
	SetActorRotation(CurrentRotation);
	
}

bool APla::ShouldMoved() const
{
	return GetDistanceMoved() > MovedDistance;
}

float APla::GetDistanceMoved() const
{
	return FVector::Dist(StartLocation, GetActorLocation());
}


void APla::MovePlatform(float DeltaTime)
{
	if (ShouldMoved())
	{
		FVector MoveDirection = PlatformVelocity.GetSafeNormal();
		StartLocation = StartLocation + MoveDirection * MovedDistance;
		SetActorLocation(StartLocation);
		PlatformVelocity = -PlatformVelocity;
	}
	else
	{
		FVector CurrentLocation = GetActorLocation();
		CurrentLocation = CurrentLocation + (PlatformVelocity * DeltaTime);
		SetActorLocation(CurrentLocation);
	}
}

