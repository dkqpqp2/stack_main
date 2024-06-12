// Fill out your copyright notice in the Description page of Project Settings.


#include "BoosterBarWidget.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "StackOBot.h"

void UBoosterBarWidget::SetPercent(float Percent)
{
	UMaterialInstanceDynamic* Dynamic = Image->GetDynamicMaterial();
	UE_LOG(LogMiniGame, Warning, TEXT("Percent : %.2f"), Percent);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("%.2f"), Percent));
	Dynamic->SetScalarParameterValue(TEXT("Percent"), Percent);
}
