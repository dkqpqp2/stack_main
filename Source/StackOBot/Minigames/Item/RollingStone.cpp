// Fill out your copyright notice in the Description page of Project Settings.


#include "RollingStone.h"
#include "Components/BoxComponent.h"
#include "Minigames/Obot/Character/MG_CharacterPlayer.h"
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

	StoneMesh->OnComponentHit.AddDynamic(this, &ARollingStone::OnHit);
	
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

void ARollingStone::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Display, TEXT("HIT"));
	AMG_CharacterPlayer* Player = Cast<AMG_CharacterPlayer>(OtherActor);
	if (OtherActor && OtherActor != this)
	{
		// Apply impulse to the OtherActor
		UPrimitiveComponent* OtherPrimitive = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
		if (OtherPrimitive)
		{
			//ARollingStone* Stone = Cast<ARollingStone>(HitComp);
			float StoneBounceForce = BounceForce;
			FVector LaunchDirection = OtherActor->GetActorLocation() - GetActorLocation();
			LaunchDirection.Normalize();
			Player -> LaunchCharacter(LaunchDirection * StoneBounceForce, true, true);
			Player -> GetMesh()->CreateDynamicMaterialInstance(1)->SetScalarParameterValue(TEXT("Mood"), 12.0f);
		}
	}
}

