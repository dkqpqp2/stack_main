// Fill out your copyright notice in the Description page of Project Settings.


#include "BoosterBarWidget.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"

void UBoosterBarWidget::SetPercent(float Percent)
{
	UMaterialInstanceDynamic* Dynamic = Image->GetDynamicMaterial();
	Dynamic->SetScalarParameterValue(TEXT("Percent"), Percent);
}
