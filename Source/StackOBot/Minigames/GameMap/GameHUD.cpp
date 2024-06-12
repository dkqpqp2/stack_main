// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHUD.h"
#include "GameFramework/PlayerController.h"
#include "MainWidget.h"
#include "Minigames/OBot/UI/MainHUD.h"

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();

}

void AGameHUD::AddMainWidget()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && MainWidgetClass)
	{
		MainWidget = CreateWidget<UMainWidget>(PlayerController, MainWidgetClass);
		MainWidget->AddToViewport();

		MainHUD = CreateWidget<UMainHUD>(GetWorld(), MainHUDClass);
		MainHUD->AddToViewport();
	}
}

void AGameHUD::AddAnnouncement()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && AnnouncementClass)
	{
		Announcement = CreateWidget<UAnnouncement>(PlayerController, AnnouncementClass);
		Announcement->AddToViewport();
	}
}

