// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Minigames/TeamChangeInterface.h"
#include "CharacterBase.h"
#include "MG_ShootingCharacterBase.generated.h"

UCLASS()
class STACKOBOT_API AMG_ShootingCharacterBase : public ACharacterBase, public ITeamChangeInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMG_ShootingCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UMaterialInstanceDynamic* MaterialDynamicInstance;

public:	
	virtual void SetMaterialToTeamColor(FVector4 NewColor) override;

};
