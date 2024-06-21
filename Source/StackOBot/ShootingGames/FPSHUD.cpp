// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSHUD.h"
#include "GameFramework/PlayerController.h"
#include "FPSOverlay.h"


void AFPSHUD::BeginPlay()
{
	Super::BeginPlay();
	AddCharacterOverlay();
}

void AFPSHUD::AddCharacterOverlay()
{
	GetOwningPlayerController(); // 플레이어 컨트롤러 가져오는 기본 함수
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && FPSOverlayClass)
	{
		FPSOverlay = CreateWidget<UFPSOverlay>(PlayerController, FPSOverlayClass);
		FPSOverlay->AddToViewport();
	}
}


void AFPSHUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2D ViewportSize;
	if (GEngine)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize); //벡터2d에 뷰포트가 저장 
		const FVector2D ViewportCenter(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);

		float SpreadScaled = CrossHariSpreadMax * HUDPackage.CrosshairSpread;

		if (HUDPackage.CrosshairsCenter)
		{
			FVector2D Spread(0.f, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsCenter, ViewportCenter,Spread);
		}
		if (HUDPackage.CrosshairsLeft)
		{
			FVector2D Spread(-SpreadScaled, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsLeft, ViewportCenter,Spread);
		}
		if (HUDPackage.CrosshairsRight)
		{
			FVector2D Spread(SpreadScaled, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsRight, ViewportCenter,Spread);
		}
		if (HUDPackage.CrosshairsTop)
		{
			FVector2D Spread(0.f, -SpreadScaled);
			DrawCrosshair(HUDPackage.CrosshairsTop, ViewportCenter,Spread);
		}
		if (HUDPackage.CrosshairsBottom)
		{
			FVector2D Spread(0.f, SpreadScaled);
			DrawCrosshair(HUDPackage.CrosshairsBottom, ViewportCenter,Spread);
		}
	}
}


void AFPSHUD::DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter,FVector2D Spread)
{
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();
	const FVector2D TextureDrawPoint(
		ViewportCenter.X - (TextureWidth / 2.f) + Spread.X,
		ViewportCenter.Y - (TextureHeight / 2.f) + Spread.Y
	);

	DrawTexture(Texture,
		TextureDrawPoint.X,
		TextureDrawPoint.Y,
		TextureWidth,
		TextureHeight,
		0.f,
		0.f,
		1.f,
		1.f,
		FLinearColor::White
		);
}
