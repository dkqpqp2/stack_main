// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Net/UnrealNetwork.h"
#include "FPSHUD.generated.h"

USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()
public:
	class UTexture2D* CrosshairsCenter;
	UTexture2D* CrosshairsLeft;
	UTexture2D* CrosshairsRight;
	UTexture2D* CrosshairsTop;
	UTexture2D* CrosshairsBottom;
	float CrosshairSpread;
};

UCLASS()
class STACKOBOT_API AFPSHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> FPSOverlayClass; //위젯 하위 클래스 

	UPROPERTY()
	class UFPSOverlay* FPSOverlay;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UFPSAnnouncement> AnnouncementClass;

	void AddAnnouncement();

	UPROPERTY()
	class UFPSAnnouncement* Announcement;

protected:
	virtual void BeginPlay() override;
	void AddCharacterOverlay();
private:
	FHUDPackage HUDPackage;

	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter,FVector2D Spread); //Center에다가 크로스 헤어 그리기 

	UPROPERTY(EditAnywhere)
	float CrossHariSpreadMax = 16.f;

public:
	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) { HUDPackage = Package; } // 인라인 화
};
