// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_EffectBase.h"

// Sets default values
AMG_EffectBase::AMG_EffectBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMG_EffectBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMG_EffectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

