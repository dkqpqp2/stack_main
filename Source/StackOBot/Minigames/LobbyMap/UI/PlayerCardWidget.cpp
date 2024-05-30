// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCardWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "../LobbyPlayerState.h"

void UPlayerCardWidget::CardUpdate(const ALobbyPlayerState* const  LobbyPS )
{
	if (LobbyPS->GetIsRedTeam())
	{
		TeamImage->SetBrushTintColor(FSlateColor(FColor::Red));
	}
	else
	{
		TeamImage->SetBrushTintColor(FSlateColor(FColor::Blue));
	}

	CharacterText->SetText(FText::FromString(LobbyPS->GetSelectedCharacter()));

	if (LobbyPS->GetIsHost())
	{
		ReadyTextBlock->SetText(FText::FromString(TEXT("Host")));

	}
	else
	{
		if (LobbyPS->GetIsReady())
		{
			ReadyTextBlock->SetText(FText::FromString(TEXT("Ready")));

		}
		else
		{
			ReadyTextBlock->SetText(FText::FromString(TEXT("Not Ready")));

		}

	}

	// 
}
