// Fill out your copyright notice in the Description page of Project Settings.


#include "Minigames/OBot/Projectile/MG_MissileTest.h"
#include "Engine.h"
#include "NiagaraFunctionLibrary.h"
#include "Minigames/OBot/Character/MG_CharacterPlayer.h"

// Sets default values
AMG_MissileTest::AMG_MissileTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;

	MissileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MissileMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("/Script/Engine.StaticMesh'/Game/LearningKit_Robots/StaticMeshes/MergedActors/MA_Robot_A2.MA_Robot_A2'"));
	if (MeshObj.Object)
	{
		MissileMesh->SetStaticMesh(MeshObj.Object);
	}
	MissileMesh->SetupAttachment(RootComponent);
	MissileMesh->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 5000.0f;
	ProjectileMovement->MaxSpeed = 30000.0f;
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

	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void AMG_MissileTest::BeginPlay()
{
	Super::BeginPlay();

	if (!HasTarget)
	{
		ProjectileMovement->Velocity = GetActorUpVector() * 150.0f;

		this->SetActorEnableCollision(false);
	}
}


void AMG_MissileTest::FindPlayer(AMG_CharacterBase* TargetActor)
{
	PlayerInWorld = TargetActor;
	Target = PlayerInWorld;
}

void AMG_MissileTest::UpdateTarget()
{
	if (!HasTarget)
	{
		if (PlayerInWorld != NULL)
		{
			if (PlayerInWorld->IsValidLowLevel())
			{
				Target = PlayerInWorld;
				HasTarget = true;
				HasNoTarget = false;

				FRotator RotVal = MissileMesh->GetComponentRotation();
				RotVal.Roll = 0.f;
				RotVal.Pitch = 0.f;
				RotVal.Yaw = 0.f;
				MissileMesh->SetRelativeRotation(RotVal);
			}
			else
			{
				Target = nullptr;
				HasTarget = true;
				HasNoTarget = true;
			}
		}
		else
		{
			Target = nullptr;
			HasTarget = true;
			HasNoTarget = true;
		}
	}
}

void AMG_MissileTest::DelayLogic(float DeltaTime)
{
	if (!HasFinishedDelay)
	{
		DelayTimer += 1 * DeltaTime;
		if (DelayTimer > 1.f)
		{
			UpdateTarget();
			this->SetActorEnableCollision(true);
			HasFinishedDelay = true;
		}
	}
}

// Called every frame
void AMG_MissileTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!HasFinishedDelay)
	{
		DelayLogic(DeltaTime);
	}
	else
	{
		if (HasTarget)
		{
			if (Target != NULL)
			{
				if (Target->IsValidLowLevel())
				{
					FVector WantedDir = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
					ProjectileMovement->Velocity = WantedDir * 30000.f * DeltaTime;
				}
				else
				{
					if (!this->IsPendingKill())
					{
						if (this->IsValidLowLevel())
						{
							K2_DestroyActor();
						}
					}
				}
			}
			else
			{
				if (HasNoTarget)
				{
					ProjectileMovement->Velocity = GetActorUpVector() * 200.f;
					HasNoTarget = false;
				}
			}
		}

		LifeTimeCountDown -= 1 * DeltaTime;

		if (LifeTimeCountDown < 0.f)
		{
			if (!CanBeDestroyed)
			{
				CanBeDestroyed = true;
				Target = nullptr;
				Explode();
			}
		}
	}
}

void AMG_MissileTest::OnOverlapBegin(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult)
{
	AMG_CharacterPlayer* PlayerCharacter = Cast<AMG_CharacterPlayer>(OtherActor);
	class AStaticMeshActor* GroundActor = Cast<AStaticMeshActor>(OtherActor);

	if (!HasAuthority())
	{
		return;
	}
	if (PlayerCharacter != nullptr)
	{
		if (!PlayerCharacter->GetIsShield())
		{
			PlayerCharacter->StunnedMood();

		}
		PlayExplosion(ExplosionSystem);
		PlayExplosionSound(ExplosionSound);
		if (this->IsValidLowLevel())
		{
			Destroy();
		}
	}
}

void AMG_MissileTest::Explode()
{
	PlayExplosion(ExplosionSystem);
	PlayExplosionSound(ExplosionSound);
	if (this->IsValidLowLevel())
	{
		Destroy();
	}
}


TObjectPtr<class UNiagaraComponent> AMG_MissileTest::PlayExplosion(UNiagaraSystem* Explosion)
{
	class UNiagaraComponent* RetVal = NULL;
	if (Explosion)
	{
		class UWorld* const World = GetWorld();

		if (World)
		{
			FVector myPos = GetActorLocation();
			FRotator myRot = GetActorRotation();

			RetVal = UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, Explosion, myPos, myRot);
		}
	}
	return RetVal;
}

TObjectPtr<class UAudioComponent> AMG_MissileTest::PlayExplosionSound(USoundCue* Sound)
{
	class UAudioComponent* RetVal = NULL;

	if (Sound)
	{
		RetVal = UGameplayStatics::SpawnSoundAttached(Sound, this->GetRootComponent());
	}

	return RetVal;
}

