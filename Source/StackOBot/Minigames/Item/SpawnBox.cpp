// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnBox.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpawnBox::ASpawnBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Collision box 만들기
	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	SpawnBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SpawnBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	RootComponent = SpawnBox;
	//overlap event bind
	SpawnBox->OnComponentBeginOverlap.AddDynamic(this, &ASpawnBox::OnOverlapbegin);

}

// Called when the game starts or when spawned
void ASpawnBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnBox::OnOverlapbegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* PlayerStart = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());
	//다른 물체가 닿으면 start 위치로 location 설정
	ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
	UE_LOG(LogTemp, Display, TEXT("overlap"));
	FVector DeathLocation = PlayerCharacter->GetActorLocation();
	if(DeathLocation.X < -12602)
	    PlayerCharacter->SetActorLocation(PlayerStart->GetActorLocation());
	else
		PlayerCharacter->SetActorLocation(HalfSpawn);
	  	
}

