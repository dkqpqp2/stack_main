// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MG_CharacterBase.h"
#include "Minigames/OBot/Interface/MG_AnimationAttackInterface.h"
#include "MG_CharacterPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishLineReached); //데디케이트 브로드캐스트 가능한 함수 정의

struct FInputActionValue;

/**
 * 
 */
UCLASS()
class STACKOBOT_API AMG_CharacterPlayer : public AMG_CharacterBase, public IMG_AnimationAttackInterface
{
	GENERATED_BODY()
public:
	AMG_CharacterPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 빙의 되었을 때 인풋매핑컨텍스트 적용해주기.
	virtual void PossessedBy(AController* NewController) override;

	bool IsHovering() const;
	UFUNCTION(Server, Unreliable)
	void ServerStartHover();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastStartHover();

	UFUNCTION(Server, Unreliable)
	void ServerLaunchCharacter();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastLaunchCharacter();

	UFUNCTION(Server, Unreliable)
	void ServerStopHover();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastStopHover();
protected:
	void ChangeCharacterControl();
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);
	virtual void SetCharacterControlData(const UMG_ControlData* CharacterControlData) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	TObjectPtr<class USkeletalMeshComponent> Jetpack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Niagara)
	TObjectPtr<class UNiagaraComponent> NiagaraEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Niagara)
	TObjectPtr<class UNiagaraSystem> NiagaraActivationEffect;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ChangeControlAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> QuaterMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> DashAction;

	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);
	void QuaterMove(const FInputActionValue& Value);

	ECharacterControlType CurrentCharacterControlType;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ActionMontage;

protected:
	UPROPERTY(EditAnywhere)
	float DashCoolTime = 2.f;

	UPROPERTY(Replicated)
	bool bCanDash = true;

	void Dash();

	UFUNCTION(Server, Reliable)
	void ServerDash();

	FTimerHandle DashDelayTimer;
	void OnDashDelayEnd();

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty>& OutLifetimeProps) const override;
	void Attack();
	virtual void AttackHitCheck() override;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAttack();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastAttack();

	UPROPERTY(ReplicatedUsing = OnRep_CanAttack)
	uint8 bCanAttack : 1;

	UFUNCTION()
	void OnRep_CanAttack();

	float AttackTime = 1.4667f;
//-------------Hover Time -----------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHoveringTime = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHoveringTime;
	
	TSubclassOf<UUserWidget> MainHUDClass;
	class UMainHUD* MainHUD;

	void StartJump();
	virtual void Jump() override;
	virtual void StopJumping() override;
private:
	bool bIsJetpackActive;
	bool bIsHovering;

	UFUNCTION()
	void JetPackUseTime(float DeltaTime);


// ---------- Item Functions ------------

protected:
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_CurrentWalkSpeed)
	float CurrentWalkSpeed = 700.f;

	void SetCurrentWalkSpeed(float NewCurrentWalkSpeed);

	UFUNCTION()
	void OnRep_CurrentWalkSpeed();
// ---------- Booster------------
protected:
	UPROPERTY(EditAnywhere)
	float BoosterDuration = 3.5f;	
	
	UPROPERTY(EditAnywhere)
	float BoosterSpeed = 1500.f;
public:
	UFUNCTION(BlueprintCallable)
	void OnBoosterItem();


protected:
	void OnBoosterEnd();

	UFUNCTION(NetMulticast, Reliable)
	void MultiCast_BoosterEffect(bool IsActivated);

	UPROPERTY(EditAnywhere, Category = Niagara)
	TObjectPtr<class UNiagaraComponent> BoosterNiagaraEffect;

	FTimerHandle WalkSpeedTimer;

// ---------- Barrier------------
public:
	void OnBarrierOverlap();
protected:
	void OnBarrierEnd();

// ---------- Shield ------------
public:
	void OnShield();
protected:
	UPROPERTY(EditAnywhere, Category = Niagara)
	TObjectPtr<class UNiagaraComponent> ShieldNiagaraEffect;

	void OnShieldEnd();
	FTimerHandle ShieldTimer;

	UPROPERTY(ReplicatedUsing = OnRep_IsShield)
	bool bIsShield = false;

	UFUNCTION()
	void OnRep_IsShield();
public:
	bool GetIsShield();
//------Character begin Overlap ------
	// 충돌 이벤트를 처리하기 위한 이벤트 디스패처
	UPROPERTY(BlueprintAssignable)
	FOnFinishLineReached OnFinishLineReached;

	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);
};
