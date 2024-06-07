// Fill out your copyright notice in the Description page of Project Settings.


#include "Minigames/Item/SlowBarrier.h"
#include "Components/BoxComponent.h"
#include "../OBot/Character/MG_CharacterPlayer.h"

// Sets default values
ASlowBarrier::ASlowBarrier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	BarrierBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BarrierBoxCollision"));
	SetRootComponent(BarrierBoxCollision);
	BarrierBoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBarrier);


	BarrierMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrierMesh"));
	BarrierMesh->SetupAttachment(RootComponent);
}

void ASlowBarrier::OnOverlapBarrier(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMG_CharacterPlayer* Character = Cast<AMG_CharacterPlayer>(OtherActor);
	if (Character && HasAuthority())
	{
		Character->OnBarrierOverlap();
	}
}

// Called when the game starts or when spawned
void ASlowBarrier::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASlowBarrier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

