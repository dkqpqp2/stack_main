// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_EffectAttack.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

AMG_EffectAttack::AMG_EffectAttack()
{
	static ConstructorHelpers::FObjectFinder<UNiagaraComponent> NiagaraRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/Character/Effect/AttackEffect.AttackEffect'"));
	if (NiagaraRef.Object)
	{
		NiagaraEffect = NiagaraRef.Object;
	}
}
