// Fill out your copyright notice in the Description page of Project Settings.


#include "RollingStone.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

// Sets default values
ARollingStone::ARollingStone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StoneCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("StoneCollision"));
	SetRootComponent(StoneCollision);
	//StoneCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapStone);


	StoneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StoneMesh"));
	StoneMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ARollingStone::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARollingStone::OnOverlapStone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult,FVector NormalImpulse)
{
	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("hIT"));
		//공이 캐릭터에 충돌 시 발생하는 이벤트 넣어도 좋을 듯
	}
}

// Called every frame
void ARollingStone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

