// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "LobbyPlayerController.h"


void ALobbyGameStateBase::SetMapIndex(int32 NewMapIndex)
{
	if (!HasAuthority())
	{
		return;
	}

	if (NewMapIndex >= GameMaps.Num())
	{
		NewMapIndex = 0;
	}
	else if (NewMapIndex < 0)
	{
		NewMapIndex = GameMaps.Num() - 1;
	}

	MapIndex = NewMapIndex;

	// get local controlled -> get lobby widget -> get mapselect widget update.
	GetWorld()->GetFirstPlayerController<ALobbyPlayerController>()->LobbyWidgetUpdate();

}



void ALobbyGameStateBase::OnRep_MapIndex()
{
	// update mapselect widget.
	GetWorld()->GetFirstPlayerController<ALobbyPlayerController>()->LobbyWidgetUpdate();

}

void ALobbyGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALobbyGameStateBase, MapIndex);
}
