// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"
//게임 모드랑 게임 스테이트 추가 해야 함 
// 
// Sets default values
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	mBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	SetRootComponent(mCollision);

	mCollision->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	mCollision->SetCollisionProfileName(TEXT("Item"));

	mBox->SetupAttachment(mCollision);

	mBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	mBox->SetIsReplicated(true); //rpc 액터

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset(TEXT("/Script/Engine.StaticMesh'/Game/StackOBot/Environment/Props/SM_Crate.SM_Crate'"));

	if (Asset.Succeeded())
		mBox->SetStaticMesh(Asset.Object);

	SetReplicates(true);
}

// Called when the game starts or when spawned
void AItemActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

