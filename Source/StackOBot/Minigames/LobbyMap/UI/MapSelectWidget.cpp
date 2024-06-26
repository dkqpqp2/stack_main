// Fill out your copyright notice in the Description page of Project Settings.


#include "MapSelectWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "../LobbyGameStateBase.h"

void UMapSelectWidget::NativeConstruct()
{
	UpdateCurrentMap();

	BeforeButton->OnClicked.AddDynamic(this, &ThisClass::OnClickBeforeButton);
	NextButton->OnClicked.AddDynamic(this, &ThisClass::OnClickNextButton);
}

void UMapSelectWidget::UpdateCurrentMap()
{
	ALobbyGameStateBase* LobbyGS = GetWorld()->GetGameState<ALobbyGameStateBase>();
	if (!IsValid(LobbyGS))
	{
		return;
	}

	if (LobbyGS->GameMaps.IsValidIndex(LobbyGS->MapIndex))
	{
		MapTextBlock->SetText(FText::FromString(
			LobbyGS->GameMaps[LobbyGS->MapIndex].MapName
		));
	}
}

void UMapSelectWidget::EnableButton()
{
	BeforeButton->SetIsEnabled(true);
	NextButton->SetIsEnabled(true);
}

void UMapSelectWidget::OnClickBeforeButton()
{
	ALobbyGameStateBase* LobbyGS = GetWorld()->GetGameState<ALobbyGameStateBase>();
	if (!IsValid(LobbyGS))
	{
		return;
	}
	LobbyGS->SetMapIndex(LobbyGS->MapIndex - 1);
}

void UMapSelectWidget::OnClickNextButton()
{
	ALobbyGameStateBase* LobbyGS = GetWorld()->GetGameState<ALobbyGameStateBase>();
	if (!IsValid(LobbyGS))
	{
		return;
	}
	LobbyGS->SetMapIndex(LobbyGS->MapIndex + 1);
}
