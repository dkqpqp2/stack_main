// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUD.h"
#include "BoosterBarWidget.h"

void UMainHUD::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMainHUD::UpdateHoveringProgress(float Percent)
{
	BoosterBar->SetPercent(Percent);
}
