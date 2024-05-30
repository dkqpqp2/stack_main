// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayerController.h"

void AGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
}

const TSubclassOf<APawn>* const AGamePlayerController::FindCharacterClass(const FString& CharacterClassName) const
{
	if (CharacterClassesMap.Contains(CharacterClassName))
	{
		return CharacterClassesMap.Find(CharacterClassName);
	}
	else
	{
		return nullptr;
	}
}
