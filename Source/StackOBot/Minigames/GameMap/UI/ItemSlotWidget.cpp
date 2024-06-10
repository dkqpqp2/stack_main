// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSlotWidget.h"
#include "Components/TextBlock.h"

void UItemSlotWidget::NewItemToItemSlot(const FItemStruct* const NewItemStruct)
{
	ItemNameTextBlock->SetText(FText::FromString( NewItemStruct->ItemString ));
}
