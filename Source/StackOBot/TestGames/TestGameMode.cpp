// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGameMode.h"

ATestGameMode::ATestGameMode()
{
}

void ATestGameMode::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, TEXT("GameMode BeginPlay"));

}
