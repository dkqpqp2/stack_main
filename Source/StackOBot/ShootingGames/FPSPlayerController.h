// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Minigames/GameMap/GamePlayerController.h"
#include "FPSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class STACKOBOT_API AFPSPlayerController : public AGamePlayerController
{
	GENERATED_BODY()

public:
	void SetHUDHealth(float CurrentHealth, float MaxHealth);

	void SetHUDHealth_1(float CurrentHealth, float MaxHealth);

	void SetHUDHealth_2(float CurrentHealth, float MaxHealth);

	void SetHUDHealth_3(float CurrentHealth, float MaxHealth);


protected:
	virtual void BeginPlay() override;

private:
	class AFPSHUD* FPSHUD;
	
	//컨트롤러에서 월드에 만들어진 character pawn -> currentHealth를 받아서 배열에 저장 
	//체력 set 할때 아닌것들은 배열에서 순차대로 배치 
};
