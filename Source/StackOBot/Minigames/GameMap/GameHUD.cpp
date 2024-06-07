// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHUD.h"
#include "GameFramework/PlayerController.h"
#include "MainWidget.h"

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();
	AddMainWidget();

}

void AGameHUD::AddMainWidget()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && MainWidgetClass)
	{
		MainWidget = CreateWidget<UMainWidget>(PlayerController, MainWidgetClass);
		MainWidget->AddToViewport();
	}
}
