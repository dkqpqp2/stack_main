// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPSAnnouncement.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API UFPSAnnouncement : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AnnouncementText;
};
