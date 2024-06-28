// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_EffectBase.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

// Sets default values
AMG_EffectBase::AMG_EffectBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NiagaraEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraEffect"));

	RootComponent = NiagaraEffect;

}



