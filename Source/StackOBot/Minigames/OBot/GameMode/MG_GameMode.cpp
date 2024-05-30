// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_GameMode.h"
#include "Minigames/OBot/Player/MG_PlayerController.h"

AMG_GameMode::AMG_GameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> OBotClassRef(TEXT("/Script/StackOBot.MG_CharacterPlayer"));
	if (OBotClassRef.Class)
	{
		DefaultPawnClass = OBotClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<AMG_PlayerController> PlayerControllerClassRef(TEXT("/Script/StackOBot.MG_PlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}
