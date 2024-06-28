// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MG_EffectBase.generated.h"

class UNiagaraComponent;

UCLASS()
class STACKOBOT_API AMG_EffectBase : public AActor
{
	GENERATED_BODY()

public:
	AMG_EffectBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Niagara)
	TObjectPtr<class UNiagaraComponent> NiagaraEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Niagara)
	TObjectPtr<class UNiagaraSystem> NiagaraActivationEffect;


};
