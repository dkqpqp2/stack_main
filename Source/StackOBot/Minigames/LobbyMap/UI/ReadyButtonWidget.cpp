// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadyButtonWidget.h"
#include "Components/Button.h"
#include "../LobbyPlayerState.h"

void UReadyButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ReadyButton->OnClicked.AddDynamic(this, &ThisClass::OnReadyButtonClicked);
}

void UReadyButtonWidget::OnReadyButtonClicked()
{
	// PS's IsReady Set?
	auto PS = GetOwningPlayerState();
	if (!IsValid(PS))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("PlayerState Not Valid"));
		return;
	}
	auto LobbyPS = Cast<ALobbyPlayerState>(PS);
	if (!IsValid(LobbyPS))
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("LobbyPlayerState Not Valid"));
		return;
	}

	LobbyPS->SetIsReady(!LobbyPS->GetIsReady());
}
