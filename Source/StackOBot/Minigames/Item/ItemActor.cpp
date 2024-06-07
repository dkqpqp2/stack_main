// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"
#include "../GameMap/GamePlayerState.h"
#include "../GameMap/CoinGame/CoinGameState.h"
#include "../GameMap/GameMapGameMode.h"
#include "StackOBot.h"
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

	mCollision->SetBoxExtent(FVector(60.f, 60.f, 60.f));
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
	mCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBoxComponentOverlapped);

}

// Called every frame
void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemActor::OnBoxComponentOverlapped(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult
)
{
	if (OtherActor->IsA<ACharacter>() && HasAuthority())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, TEXT("Box Overlapped with character"));
		AGamePlayerState* PS = Cast<ACharacter>(OtherActor)->GetPlayerState<AGamePlayerState>();
		if (!IsValid(PS))
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("PS Not Valid : ItemActor Overlapped"));
			return;
		}
		////player score add.
		//PS->TrySetScore(PS->GetScore() + 1.f);

		// team score add.
		auto* CoinGS = GetWorld()->GetGameState<ACoinGameState>();
		if (!IsValid(CoinGS))
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("GS Not Valid : ItemActor Overlapped"));
			return;
		}
		
		PS->SetCurrentItem(EItem::E_BOOSTER);

		

		// Player State의 item에 넣기.


		Destroy();
	}
}

