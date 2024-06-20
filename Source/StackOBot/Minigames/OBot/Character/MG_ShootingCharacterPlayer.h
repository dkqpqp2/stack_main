// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MG_ShootingCharacterBase.h"
#include "../../../ShootingGames/Weapons/WeaponBase.h"
#include "MG_ShootingCharacterPlayer.generated.h"


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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TObjectPtr<class AWeaponBase> CurrentWeaponBase;

	UFUNCTION(BlueprintCallable)
	void SetHUDCrossHair(float DeltaTime); // 크로스 헤어 틱마다 불러오는 함수 


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int32 TotalAmmos = 0;

	float CrosshairVelocityFactor;
};
