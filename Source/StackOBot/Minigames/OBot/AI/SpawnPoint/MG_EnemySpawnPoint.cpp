// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_EnemySpawnPoint.h"
#include "Minigames/OBot/AI/MG_EnemyBase.h"

// Sets default values
AMG_EnemySpawnPoint::AMG_EnemySpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	RootComponent = Root;

	//Root->bVisualizeComponent = true;

	SpawnEnemy = nullptr;
	AccTime = 0.f;
	SpawnTime = 0.f;
}

// Called when the game starts or when spawned
void AMG_EnemySpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMG_EnemySpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!SpawnEnemy)
	{
		AccTime += DeltaTime;
		if (AccTime >= SpawnTime)
		{
			Spawn();
		}
	}
}

void AMG_EnemySpawnPoint::Spawn()
{
	if (!HasAuthority())
	{
		return;
	}
	if (!IsValid(SpawnClass))
	{
		return;
	}
	FActorSpawnParameters SpawnParam;

	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	SpawnEnemy = GetWorld()->SpawnActor<AMG_EnemyBase>(SpawnClass, GetActorLocation(), GetActorRotation(), SpawnParam);
	SpawnEnemy->SetItemSpawnPoint(this);
}

void AMG_EnemySpawnPoint::ClearSpawnObject()
{
	SpawnEnemy = nullptr;
	AccTime = 0.f;
}

