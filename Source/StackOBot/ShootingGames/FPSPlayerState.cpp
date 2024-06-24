// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSPlayerState.h"
#include "FPSPlayerController.h"
#include "Minigames/OBot/Character/MG_ShootingCharacterPlayer.h"

void AFPSPlayerState::BeginPlay()
{
    Super::BeginPlay();
	//OnPawnSet.AddDynamic(this, &AMG_ShootingCharacterPlayer::UpdateHUDHealth());
}
