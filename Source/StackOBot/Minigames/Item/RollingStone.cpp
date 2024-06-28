// Fill out your copyright notice in the Description page of Project Settings.


#include "RollingStone.h"
#include "Components/BoxComponent.h"
#include "Minigames/Obot/Character/MG_CharacterPlayer.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ARollingStone::ARollingStone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StoneCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("StoneCollision"));
	SetRootComponent(StoneCollision);

	StoneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StoneMesh"));
	StoneMesh->SetupAttachment(RootComponent);
	StoneMesh->SetSimulatePhysics(true);

	
}

// Called when the game starts or when spawned
void ARollingStone::BeginPlay()
{
	Super::BeginPlay();

	StoneMesh->OnComponentHit.AddDynamic(this, &ARollingStone::OnHit);
	
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
	if (!IsValid(Player))
	{
		return;
	}
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
			//Player -> GetMesh() -> CreateDynamicMaterialInstance(1)->SetScalarParameterValue(TEXT("Mood"), 12.0f);
			ChangeFace(Player, 12.0f);
			// 1초 후에 Mood 값을 다시 1.0f로 설정하기 위해 타이머 설정
		}
	}
}

void ARollingStone::ChangeFace(AActor* OtherActor, float indexnumber) 
{
	AMG_CharacterPlayer* Player = Cast<AMG_CharacterPlayer>(OtherActor);
	Player->GetMesh()->CreateDynamicMaterialInstance(1)->SetScalarParameterValue(TEXT("Mood"), indexnumber);
	OnChangeFace.Broadcast(OtherActor, indexnumber);
}

void ARollingStone::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
}


