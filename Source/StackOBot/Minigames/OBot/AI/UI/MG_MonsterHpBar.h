// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MG_UserWidget.h"
#include "MG_MonsterHpBar.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API UMG_MonsterHpBar : public UMG_UserWidget
{
	GENERATED_BODY()
public:
	UMG_MonsterHpBar(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }
	void UpdateHpBar(float NewCurrentHp);

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	float MaxHp;

};
