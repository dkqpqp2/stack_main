// Fill out your copyright notice in the Description page of Project Settings.


#include "Minigames/OBot/Actors/Podium.h"
#include "Camera/CameraComponent.h"

// Sets default values
APodium::APodium()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	PodiumMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PodiumMesh"));
	SetRootComponent(PodiumMesh);

	PlayerRank1Location = CreateDefaultSubobject<USceneComponent>(TEXT("PlayerRank1Location"));
	PlayerRank2Location = CreateDefaultSubobject<USceneComponent>(TEXT("PlayerRank2Location"));
	PlayerRank3Location = CreateDefaultSubobject<USceneComponent>(TEXT("PlayerRank3Location"));
	PlayerRank4Location = CreateDefaultSubobject<USceneComponent>(TEXT("PlayerRank4Location"));

	PlayerRank1Location->SetupAttachment(RootComponent);
	PlayerRank2Location->SetupAttachment(RootComponent);
	PlayerRank3Location->SetupAttachment(RootComponent);
	PlayerRank4Location->SetupAttachment(RootComponent);

	PlayerLocations.Add(PlayerRank1Location);
	PlayerLocations.Add(PlayerRank2Location);
	PlayerLocations.Add(PlayerRank3Location);
	PlayerLocations.Add(PlayerRank4Location);

	PodiumCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PodiumCamera"));
	PodiumCamera->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APodium::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APodium::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

