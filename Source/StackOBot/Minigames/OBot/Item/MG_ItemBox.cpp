// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_ItemBox.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "StackOBot.h"
#include "Net/UnrealNetwork.h"
#include "MG_ItemSpawnPoint.h"
#include "StackOBot.h"
#include "../../GameMap/GamePlayerState.h"
#include "../Character/MG_CharacterBase.h"


// Sets default values
AMG_ItemBox::AMG_ItemBox()
{
	PrimaryActorTick.bCanEverTick = true;

	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerShpere"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Effect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect"));

	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);
	Effect->SetupAttachment(Trigger);

	Trigger->SetCollisionProfileName(TEXT("Item"));
	Trigger->SetSphereRadius(100.0f);
	Trigger->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	Trigger->SetRelativeLocation(FVector(0.0, 0.0, 100.0));
	Trigger->SetRelativeRotation(FRotator(0.0, 0.0, 45.0));
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AMG_ItemBox::OnOverlapBegin);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/StackOBot/Environment/Props/SM_Crate.SM_Crate'"));
	if (BoxMeshRef.Object)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}
	Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, -60.0f));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> EffectRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/KTP_Effect/Particles/Fly/Others/energy_03_01.energy_03_01'"));
	if (EffectRef.Object)
	{
		Effect->SetAsset(EffectRef.Object);
		Effect->bAutoActivate = false;
	}

	bReplicates = true;
	SpawnPoint = nullptr;


}

void AMG_ItemBox::BeginPlay()
{
	Super::BeginPlay();


}

void AMG_ItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		AddActorLocalRotation(FRotator(0.0f, RotationRate * DeltaTime, RotationRate * DeltaTime));
		ServerRotationYaw = RootComponent->GetComponentRotation().Yaw;
		ServerRotationRoll = RootComponent->GetComponentRotation().Roll;
	}
	else
	{
		
	}
}

void AMG_ItemBox::OnOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (OtherActor->IsA<AMG_CharacterBase>() && HasAuthority())
	{
		NetMulticast_ItemBoxEffect();
		Effect->OnSystemFinished.AddDynamic(this, &AMG_ItemBox::OnEffectFinished);

		// 아이템을 캐릭터에 추가.
		GetNewItemTo(OtherActor);
	}
}

void AMG_ItemBox::NetMulticast_ItemBoxEffect_Implementation()
{
	Effect->Activate(true);
	Mesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);
}

void AMG_ItemBox::OnEffectFinished(UNiagaraComponent* NiagaraSystem)
{
	if (SpawnPoint)
	{
		SpawnPoint->ClearSpawnObject();
	}
	Destroy();
	
}

void AMG_ItemBox::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMG_ItemBox, ServerRotationYaw);
	DOREPLIFETIME(AMG_ItemBox, ServerRotationRoll);

}

void AMG_ItemBox::OnRep_ServerRotationYaw()
{
	FRotator NewRotator = RootComponent->GetComponentRotation();
	NewRotator.Yaw = ServerRotationYaw;
	RootComponent->SetWorldRotation(NewRotator);
}

void AMG_ItemBox::OnRep_ServerRotationRoll()
{
	FRotator NewRotator = RootComponent->GetComponentRotation();
	NewRotator.Roll = ServerRotationRoll;
	RootComponent->SetWorldRotation(NewRotator);
}

void AMG_ItemBox::GetNewItemTo(AActor* OtherActor)
{
	if (OtherActor->IsA<AMG_CharacterBase>() && HasAuthority())
	{
		AGamePlayerState* PS = Cast<AMG_CharacterBase>(OtherActor)->GetPlayerState<AGamePlayerState>();
		if (!IsValid(PS))
		{
			return;
		}

		PS->SetCurrentItemToRandomItem();
	}
}



