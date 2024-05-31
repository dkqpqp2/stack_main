// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "../ThirdPersonCharacter.h"
#include "../TeamChangeInterface.h"
#include "LobbyPlayerController.h"


void ALobbyPlayerState::BeginPlay()
{
	Super::BeginPlay();
	OnPawnSet.AddDynamic(this, &ThisClass::SetPlayerPawn);
}

void ALobbyPlayerState::SetPlayerEnterID(int32 NewEnterID)
{
	PlayerEnterID = NewEnterID;
}

void ALobbyPlayerState::SetIsRedTeamTo(bool IsChecked)
{
	//if server
	if (HasAuthority())
	{
		// 그냥 값 변경
		IsRedTeam = IsChecked;
		// OnIsRedTeamChanged()호출해서 자신의 화면 업데이트.
		OnIsRedTeamChanged();
	}
	else
	{
		// 서버로 전송. 
		SV_SetIsRedTeamTo(IsChecked);
	}
}

void ALobbyPlayerState::SV_SetIsRedTeamTo_Implementation(bool IsChecked)
{
	// 값 변경
	IsRedTeam = IsChecked;
	// 화면 업데이트.
	OnIsRedTeamChanged();
}

void ALobbyPlayerState::OnRep_IsRedTeam()
{
	// 화면 업데이트 함수 호출
	OnIsRedTeamChanged();

	//Widget Also Changes.
	UpdatePlayerListWidget();

}

void ALobbyPlayerState::OnIsRedTeamChanged()
{
	ITeamChangeInterface* Pawn = Cast<ITeamChangeInterface>( GetPawn() );
	if (Pawn == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Pawn Does Not Implemets ITeamChangeInterface"));
		return;
	}
	Pawn->SetMaterialToTeamColor(IsRedTeam);

	//Widget Also Changes.

	UpdatePlayerListWidget();

}

void ALobbyPlayerState::SetSelectedCharacter(FString NewCharacter)
{
	// If Server, Just Do it
// else Client, Pass to Server.
	if (HasAuthority())
	{
		SelectedCharacter = NewCharacter;
		OnChangeCharacter();

	}
	else
	{
		SV_RequestChangeCharacter(NewCharacter);
	}
}

void ALobbyPlayerState::SV_RequestChangeCharacter_Implementation(const FString& NewCharacterName)
{
	SelectedCharacter = NewCharacterName;
	OnChangeCharacter();
}

void ALobbyPlayerState::OnChangeCharacter()
{
	// Only In Server.
	// call controller's change character?
	ALobbyPlayerController* LobbyPC = Cast<ALobbyPlayerController>(GetPlayerController());
	if (!IsValid(LobbyPC))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("PlayerController Not Available : OnChangeCharacter()"));
		return;
	}
	LobbyPC->ChangeCharacter(SelectedCharacter);

	//Change UI
	UpdatePlayerListWidget();
}

void ALobbyPlayerState::OnRep_SelectedCharacter()
{
	// on client. change ui
	UpdatePlayerListWidget();
}


// ----------------- IsHost, IsReady ------------------------------

void ALobbyPlayerState::OnRep_bIsReady()
{
	// Call On Ready Change..
	OnReadyChanged();
}

void ALobbyPlayerState::SV_RequestSetIsReady_Implementation(const bool IsReady)
{
	bIsReady = IsReady;
	OnReadyChanged();
}

void ALobbyPlayerState::OnReadyChanged()
{
	// change Widget's Readytext...
	UpdatePlayerListWidget();
}

void ALobbyPlayerState::SetIsReady(bool bNewIsReady)
{
	if (HasAuthority())
	{
		bIsReady = bNewIsReady;
		// Call On Ready Change...
		OnReadyChanged();
	}
	else
	{
		// send to server... mostly here.
		SV_RequestSetIsReady(bNewIsReady);
	}
}

bool ALobbyPlayerState::GetIsReady() const
{
	return bIsReady;
}


void ALobbyPlayerState::SetIsHost(bool IsHost)
{
	bIsHost = IsHost;
}
bool ALobbyPlayerState::GetIsHost() const
{
	return bIsHost;
}
// ----------------- IsHost, IsReady ------------------------------


void ALobbyPlayerState::UpdatePlayerListWidget()
{
	AController* PC = GetWorld()->GetFirstPlayerController();

	if (PC == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("No Controller : LobbyPS -> UpdatePlayerListWidget()")); //BP.... (No StaticClass(). It Returns Pawn
		return;
	}

	// call PC's edit widget.
	ALobbyPlayerController* LobbyPC = Cast<ALobbyPlayerController>(PC);
	if (!IsValid(LobbyPC))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("LobbyPC Is Not Valid : LobbyPS -> UpdatePlayerListWidget()")); //BP.... (No StaticClass(). It Returns Pawn
		return;
	}
	LobbyPC->LobbyWidgetUpdate();

}



void ALobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALobbyPlayerState, bIsHost);
	DOREPLIFETIME(ALobbyPlayerState, bIsReady);
}

void ALobbyPlayerState::SetPlayerPawn(APlayerState* Player, APawn* NewPawn, APawn* OldPawn)
{
	if (!HasAuthority())
	{
		ITeamChangeInterface* LobbyPawn = Cast<ITeamChangeInterface>(NewPawn);
		if (LobbyPawn == nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Pawn Does Not Implemets ITeamChangeInterface"));
			return;
		}
		LobbyPawn->SetMaterialToTeamColor(IsRedTeam);
		
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("OnPawnSet"));
	}

	ALobbyPlayerController* LobbyPC = Cast<ALobbyPlayerController>(GetWorld()->GetFirstPlayerController());
	if (!IsValid(LobbyPC))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("LobbyPC Is Not Valid : LobbyPS -> UpdatePlayerListWidget()")); //BP.... (No StaticClass(). It Returns Pawn
		return;
	}
	LobbyPC->LobbyWidgetUpdate();
}
