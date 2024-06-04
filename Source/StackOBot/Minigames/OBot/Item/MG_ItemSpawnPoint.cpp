// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_ItemSpawnPoint.h"
#include "Components/ArrowComponent.h"
#include "Minigames/OBot/Item/MG_ItemBox.h"

// Sets default values
AMG_ItemSpawnPoint::AMG_ItemSpawnPoint()
{
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

	Spawn();

}

// Called every frame
void AMG_ItemSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMG_ItemSpawnPoint::Spawn()
{
	if (!IsValid(SpawnClass))
	{
		return;
	}
	FActorSpawnParameters SpawnParam;

	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AMG_ItemBox* DefaultObj = Cast<AMG_ItemBox>(SpawnClass->GetDefaultObject());
	
	FVector HalfHeight;

	if (IsValid(DefaultObj))
	{
		HalfHeight = DefaultObj->GetActorLocation();
	}

	SpawnBox = GetWorld()->SpawnActor<AMG_ItemBox>(SpawnClass, GetActorLocation() + HalfHeight, GetActorRotation(), SpawnParam);
	SpawnBox->SetItemSpawnPoint(this);
}

