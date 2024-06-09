// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_PlayerController.h"
#include "Blueprint/UserWidget.h"

AMG_PlayerController::AMG_PlayerController()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> PlayerHUDClassRef(TEXT("/Game/Character/UI/WB_HUD.WB_HUD_C"));

	if (PlayerHUDClassRef.Succeeded())
	{
		PlayerHUDClass = PlayerHUDClassRef.Class;
	}
}

void AMG_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerHUD = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDClass);
	PlayerHUD->AddToViewport();

	

	
}

void AMG_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
