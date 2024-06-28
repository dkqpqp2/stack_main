// Fill out your copyright notice in the Description page of Project Settings.


#include "FinishLineBox.h"
#include "Minigames/OBot/GameMode/MG_GameMode.h"
#include "Minigames/OBot/Character/MG_CharacterPlayer.h"
#include "Minigames/OBot/Character/MG_ShootingCharacterPlayer.h"

// Sets default values
AFinishLineBox::AFinishLineBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FinishLineBox = CreateDefaultSubobject<UBoxComponent>(TEXT("FinishLine"));
	RootComponent = FinishLineBox;
}

// Called when the game starts or when spawned
void AFinishLineBox::BeginPlay()
{
	Super::BeginPlay();
	FinishLineBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapFinishLine);
}

void AFinishLineBox::OnOverlapFinishLine(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority() && (!IsGoaled))
	{
		if (!OtherActor->IsA<AMG_CharacterPlayer>())
		{
			return;
		}

		AMG_GameMode* GM = Cast<AMG_GameMode> (GetWorld()->GetAuthGameMode() );
		if (!IsValid(GM))
		{
			return;
		}
		
		GM->OnPlayerFinishLineReached();
		IsGoaled = true;
	}
}

// Called every frame
void AFinishLineBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


