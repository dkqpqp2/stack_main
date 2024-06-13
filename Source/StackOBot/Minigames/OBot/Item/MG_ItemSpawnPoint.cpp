// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_ItemSpawnPoint.h"
#include "Components/ArrowComponent.h"
#include "MG_ItemBox.h"
#include "StackOBot.h"

// Sets default values
AMG_ItemSpawnPoint::AMG_ItemSpawnPoint()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

#if WITH_EDITORONLY_DATA
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Root);
#endif

	RootComponent = Root;

	Root->bVisualizeComponent = true;

	SpawnBox = nullptr;
	AccTime = 0.f;
	SpawnTime = 0.f;
}

// Called when the game starts or when spawned
void AMG_ItemSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void AMG_ItemSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!SpawnBox)
	{
		AccTime += DeltaTime;
		if (AccTime >= SpawnTime)
		{
			Spawn();
		}
	}



}

void AMG_ItemSpawnPoint::Spawn()
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

	SpawnBox = GetWorld()->SpawnActor<AMG_ItemBox>(SpawnClass, GetActorLocation(), GetActorRotation(), SpawnParam);
	SpawnBox->SetItemSpawnPoint(this);
}

void AMG_ItemSpawnPoint::ClearSpawnObject()
{
	SpawnBox = nullptr;
	AccTime = 0.f;
}

