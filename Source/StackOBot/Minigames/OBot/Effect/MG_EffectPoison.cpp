// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_EffectPoison.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

AMG_EffectPoison::AMG_EffectPoison()
{
	static ConstructorHelpers::FObjectFinder<UNiagaraComponent> NiagaraRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/Character/Effect/PoisonEffect.PoisonEffect'"));
	if (NiagaraRef.Object)
	{
		NiagaraEffect = NiagaraRef.Object;
	}
}
