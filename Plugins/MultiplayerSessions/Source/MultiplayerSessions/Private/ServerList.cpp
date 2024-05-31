// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerList.h"
#include "Menu.h"
#include "MultiplayerSessionsSubsystem.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"

void UServerList::NativeConstruct()
{
	//Super::NativeConstruct();
}

void UServerList::PopulateServer(const TArray<FServerDel>& ServerList)
{
}
