// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameStateBase.h"
#include "Net/UnrealNetwork.h"


void ALobbyGameStateBase::OnRep_MapIndex()
{
	// update mapselect widget.
}

void ALobbyGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALobbyGameStateBase, MapIndex);
}
