// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Announcement.h"
#include "GameHUD.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API AGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> MainWidgetClass;
	void AddMainWidget(); //따로 viewport추가 해주는 함수 

	UPROPERTY()
	class UMainWidget* MainWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAnnouncement> AnnouncementClass;

	UPROPERTY()
	class UAnnouncement* Announcement;
	void AddAnnouncement();

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> MainHUDClass;

	class UMainHUD* MainHUD;

protected:
	virtual void BeginPlay() override;

};
