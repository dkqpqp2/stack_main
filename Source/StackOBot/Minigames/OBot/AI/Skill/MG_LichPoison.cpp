// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_LichPoison.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Components/BoxComponent.h"
#include "Minigames/OBot/Character/MG_ShootingCharacterPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMG_LichPoison::AMG_LichPoison()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	PoisonBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PoisonBox"));

	RootComponent = PoisonBox;

	PoisonActor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PoisonActor"));

	PoisonActor->SetupAttachment(RootComponent);
	PoisonActor->SetRelativeLocationAndRotation(FVector(0.0, -175.0, 5.0), FRotator(0.0, 0.0, 90.0));
	PoisonBox->SetBoxExtent(FVector(275.0, 150.0, 10.0));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PoisonMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Character/AI/Lich/MG_LichPoison.MG_LichPoison'"));
	if (PoisonMeshRef.Object)
	{
		PoisonActor->SetStaticMesh(PoisonMeshRef.Object);
	}
}

// Called when the game starts or when spawned
void AMG_LichPoison::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(10.0f);
	GetWorldTimerManager().SetTimer(PoisonTimer, this, &AMG_LichPoison::ApplyPoisonDamage, 1.0f, true);
	PoisonBox->OnComponentBeginOverlap.AddDynamic(this, &AMG_LichPoison::OverlapBeginPoison);
	PoisonBox->OnComponentEndOverlap.AddDynamic(this, &AMG_LichPoison::OverlapEndPoison);
}

void AMG_LichPoison::OverlapBeginPoison(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMG_ShootingCharacterPlayer* ShootingPlayer = Cast<AMG_ShootingCharacterPlayer>(OverlappedComponent->GetOwner());
	ShootingPlayer->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
}

void AMG_LichPoison::OverlapEndPoison(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMG_ShootingCharacterPlayer* ShootingPlayer = Cast<AMG_ShootingCharacterPlayer>(OverlappedComponent->GetOwner());
	ShootingPlayer->GetCharacterMovement()->MaxWalkSpeed = 500.0f;
}

// Called every frame
void AMG_LichPoison::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMG_LichPoison::ApplyPoisonDamage()
{
	TArray<AActor*> Players;
	TSubclassOf<AMG_ShootingCharacterPlayer> CharacterPlayerClass = AMG_ShootingCharacterPlayer::StaticClass();
	PoisonBox->GetOverlappingActors(Players, CharacterPlayerClass);
	for (auto Player : Players)
	{
		UGameplayStatics::ApplyDamage(Player, PoisonDamage, nullptr, this, UDamageType::StaticClass());
	}
	
}



