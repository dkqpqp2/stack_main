// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpPad.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Minigames/OBot/Character/MG_CharacterPlayer.h"
#include "GameFramework/Character.h"
// Sets default values
AJumpPad::AJumpPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// jump pad need tick??


	JumpPadCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("JumpPadCollision"));
	SetRootComponent(JumpPadCollision);
	JumpPadCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapJumpPad);


	JumpPadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("JumpPadMesh"));
	JumpPadMesh->SetupAttachment(RootComponent);


	// Set default values
	

	JumpDirectionArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("JumpDirectionArrow"));
	JumpDirectionArrow->SetupAttachment(RootComponent);

}


void AJumpPad::OnOverlapJumpPad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (Character)
	{
		// Get Arrow's Forward Vector.
		FVector LaunchVelocity = JumpDirectionArrow->GetForwardVector() * LaunchStrength;
		Character->LaunchCharacter(LaunchVelocity, false, true);
	}
}

// Called when the game starts or when spawned
void AJumpPad::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJumpPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

