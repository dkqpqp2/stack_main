// Fill out your copyright notice in the Description page of Project Settings.


#include "StartButtonWidget.h"
#include "Components/Button.h"
#include "../LobbyGameMode.h"

void UStartButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();
	StartButton->OnClicked.AddDynamic(this, &ThisClass::OnStartButtonClicked);

}

void UStartButtonWidget::OnStartButtonClicked()
{
	// do i have to check, here is server again?
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Magenta, TEXT("StartButton Clicked"));
	ALobbyGameMode* LobbyGM = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (!IsValid(LobbyGM))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("LobbyGM Is Not Valid : StartButton Clicked"));
		return;
	}
	LobbyGM->ServerTravelToGameMap();
}
