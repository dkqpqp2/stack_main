// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MultiplayerSessionsSubsystem.h"
#include "Components/EditableTextBox.h"
#include "ServerList.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UServerList : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void PopulateServer(const TArray<FServerDel>& ServerList);

private:
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* TB_ServerName;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* TB_Players;


};
