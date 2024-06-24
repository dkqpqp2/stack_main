// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingGames/Weapons/WeaponBase.h"
#include "Minigames/OBot/Character/MG_ShootingCharacterPlayer.h"
#include "ShootingGames/FPSPlayerController.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeaponBase, CurrentAmmo);
	
}

void AWeaponBase::SetHUDBullet()
{
	FPSCharacter = FPSCharacter == nullptr ? Cast<AMG_ShootingCharacterPlayer>(GetOwner()) : FPSCharacter;
	if (FPSCharacter)
	{
		FPSPlayerController = FPSPlayerController == nullptr ? Cast<AFPSPlayerController>(FPSCharacter->Controller) : FPSPlayerController;
		if (FPSPlayerController)
		{
			FPSPlayerController->SetHUDAmmo(CurrentAmmo,MaxAmmo); //캐릭터랑 컨트롤러 가 있을때 hud set 
		}
	}
}

void AWeaponBase::On_RepAmmo()
{
	SetHUDBullet();
}

