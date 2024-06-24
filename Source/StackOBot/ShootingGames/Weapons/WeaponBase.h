// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8 
{ 
	Rifle UMETA(DisplayName = "Rifle"), 
	Pistol UMETA(DisplayName = "Pistol"), 
	Grenade UMETA(DisplayName = "Grenade"), 
};

UCLASS()
class STACKOBOT_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/** Returns the properties used for network replication, this needs to be overridden by all actor classes with native replicated properties */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


public:
	UPROPERTY(EditAnywhere, Category = CrossHairs)
	class UTexture2D* CrosshairsCenter;

	UPROPERTY(EditAnywhere, Category = CrossHairs)
	class UTexture2D* CrosshairsLeft;

	UPROPERTY(EditAnywhere, Category = CrossHairs)
	class UTexture2D* CrosshairsRight;

	UPROPERTY(EditAnywhere, Category = CrossHairs)
	class UTexture2D* CrosshairsTop;

	UPROPERTY(EditAnywhere, Category = CrossHairs)
	class UTexture2D* CrosshairsBottom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite)
	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CoolTime;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite)
	TObjectPtr<class AMG_ShootingCharacterPlayer> WeaponOwner;
};
