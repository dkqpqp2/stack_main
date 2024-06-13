// Fill out your copyright notice in the Description page of Project Settings.


#include "Minigames/OBot/Projectile/MG_MissileTest.h"
#include "Engine.h"

// Sets default values
AMG_MissileTest::AMG_MissileTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;

	MissileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MissileMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT(""));
	if (MeshObj.Object)
	{
		MissileMesh->SetStaticMesh(MeshObj.Object);
	}
	MissileMesh->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 200.0f;
	ProjectileMovement->MaxSpeed = 300.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bInitialVelocityInLocalSpace = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->bIsHomingProjectile = false;
	ProjectileMovement->HomingAccelerationMagnitude = 0.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->Velocity = FVector(0.0, 0.0, 0.0);

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AMG_MissileTest::OnOverlapBegin);

	HasTarget = false;
	HasNoTarget = false;
	Target = NULL;
	DelayTimer = 0.f;
	HasFinishedDelay = false;
	LifeTimeCountDown = 15.f;
	CanBeDestroyed = false;
	PlayerInWorld = NULL;
}

// Called when the game starts or when spawned
void AMG_MissileTest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMG_MissileTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMG_MissileTest::DelayLogic(float DeltaTiem)
{
}

void AMG_MissileTest::FindPlayer()
{
}

void AMG_MissileTest::UpdateTarget()
{
}

void AMG_MissileTest::OnOverlapBegin(UPrimitiveComponent* OverlapComp, AActor* OtherActro, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult)
{
}

void AMG_MissileTest::Explode()
{
}


TObjectPtr<class UNiagaraComponent> AMG_MissileTest::PlayExplosion(UNiagaraSystem* Explosion)
{
	return TObjectPtr<class UNiagaraComponent>();
}

TObjectPtr<class UAudioComponent> AMG_MissileTest::PlayExplosionSound(USoundCue* Sound)
{
	return TObjectPtr<class UAudioComponent>();
}

