// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPSOverlay.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API UFPSOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar; //본인 player

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar_1; //player1 체력

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar_2; // ""2

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar_3; // "" 3

	UPROPERTY(meta = (BindWidget))	
	class UTextBlock* HealthText; 

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Player1; // player1 이름

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Player2;  // Player2 이름

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Player3; //Player3 이름

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* BulletText; //총알 문자 

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentBullet; //현재 총알

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MaxBullet; //최대 총알 

};
