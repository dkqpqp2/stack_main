// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MG_ShootingCharacterBase.h"
#include "Net/UnrealNetwork.h"
#include "../../../ShootingGames/Weapons/WeaponBase.h"
#include "MG_ShootingCharacterPlayer.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishLineReached1); //데디케이트 브로드캐스트 가능한 함수 정의

struct FInputActionValue;
/**
 * 
 */
UCLASS()
class STACKOBOT_API AMG_ShootingCharacterPlayer : public AMG_ShootingCharacterBase
{
	GENERATED_BODY()
public:
	AMG_ShootingCharacterPlayer();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	TObjectPtr<class USkeletalMeshComponent> Jetpack;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> CrouchAction;

	// Test HpBar
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	UPROPERTY(EditAnywhere,Category = CrossHairs)
	class UTexture2D* CrosshairsCenter;

	UPROPERTY(EditAnywhere, Category = CrossHairs)
	class UTexture2D* CrosshairsLeft;

	UPROPERTY(EditAnywhere, Category = CrossHairs)
	class UTexture2D* CrosshairsRight;

	UPROPERTY(EditAnywhere, Category = CrossHairs)
	class UTexture2D* CrosshairsTop;

	UPROPERTY(EditAnywhere, Category = CrossHairs)
	class UTexture2D* CrosshairsBottom;

	class AFPSHUD* HUD;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void OnCrouch(const FInputActionValue& Value);
	void OffCrouch(const FInputActionValue& Value);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool HasWeapon = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Weapon)
	TObjectPtr<class AWeaponBase> CurrentWeaponBase;

	UFUNCTION()
	void OnRep_Weapon();

	UFUNCTION(BlueprintCallable)
	void SetHUDCrossHair(float DeltaTime); // 크로스 헤어 틱마다 불러오는 함수 

	// 안쓰게 될지도.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int32 TotalAmmos = 0;

	float CrosshairVelocityFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CrosshairShootingFactor = 0.f;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int32 RifleAmmos = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int32 PistolAmmos = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int32 GrenadeAmmos = 0;

	//플레이어 체력, 이름 
public:
	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_Health)
	float CurrentHealth = 100.f;

	UFUNCTION()
	void OnRep_Health();

	UFUNCTION(BlueprintCallable)
	void SetCurrentHealthWithHUDUpdate(float NewHealth);

	UPROPERTY(EditAnywhere, Replicated)
	FString PlayerName = "";
	

protected:
	UFUNCTION(BlueprintCallable)
	void TryReloadWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Replicated)
	bool IsReloading = false;

	UFUNCTION(NetMulticast, Reliable)
	void PlayReloadStartAnimation();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> ReloadAnimMontage;

public:
	UFUNCTION(BlueprintCallable)
	void OnEndReloadAnimation();


	UFUNCTION()
	void ReceiveDamage(AActor* DamagaedActor, float Damagae, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);

	void UpdateHUDHealth();

	class AFPSPlayerController* FPSPlayerController;

protected:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_RefreshHP();

protected:
	void OnDeathPlayRagdoll();
	FTimerHandle DeathTimer;
	void OnDeathEnd();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayRagdoll();

//------Character begin Overlap ------
// 충돌 이벤트를 처리하기 위한 이벤트 디스패처
public:
	UPROPERTY(BlueprintAssignable)
	FOnFinishLineReached1 OnFinishLineReached1;

	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	bool IsWinner = false;
};
