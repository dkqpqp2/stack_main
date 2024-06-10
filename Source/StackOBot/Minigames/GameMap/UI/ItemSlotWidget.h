// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../Item/ItemBoxItemEnum.h"
#include "ItemSlotWidget.generated.h"


class UTextBlock;
/**
 * 
 */
UCLASS()
class STACKOBOT_API UItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta= (BindWidget))
	TObjectPtr<UTextBlock> ItemNameTextBlock;
	
public:
	void NewItemToItemSlot(EItem NewItem);
};
