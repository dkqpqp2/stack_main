// Fill out your copyright notice in the Description page of Project Settings.


#include "StartLineBox.h"

// Sets default values
AStartLineBox::AStartLineBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StartLineBox = CreateDefaultSubobject<UBoxComponent>(TEXT("StartLine"));
	RootComponent = StartLineBox;
	//StartLine -> 충돌 채널 설정
	Line = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LineMesh"));
	Line->SetupAttachment(StartLineBox);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("/Game/LearningKit_Robots/StaticMeshes/Robot/SM_KitRobot_Rail.SM_KitRobot_Rail"));
	if (SM.Succeeded())
	{
		Line->SetStaticMesh(SM.Object);
	}
}

// Called when the game starts or when spawned
void AStartLineBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStartLineBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

