// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_CharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MG_ControlData.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "StackOBot.h"
#include "Animation/AnimMontage.h"
#include "Net/UnrealNetwork.h"
#include "NiagaraFunctionLibrary.h"
#include "Minigames/OBot/UI/MainHUD.h"
#include "Minigames/GameMap/GameHUD.h"

AMG_CharacterPlayer::AMG_CharacterPlayer()
{
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->TargetArmLength = 400.f;
	CameraArm->bUsePawnControlRotation = true;


	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	Jetpack = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Jetpack"));
	Jetpack->SetupAttachment(GetMesh(), FName("BackpackSocket"));

	NiagaraEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraEffect"));
	NiagaraEffect->SetupAttachment(Jetpack);

	BoosterNiagaraEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BoosterNiagaraEffect"));
	BoosterNiagaraEffect->SetupAttachment(Jetpack);

	ShieldNiagaraEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ShieldNiagaraEffect"));
	ShieldNiagaraEffect->SetupAttachment(GetMesh(), FName("ShieldSocket"));
	ShieldNiagaraEffect->Deactivate();

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> JetpackMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/StackOBot/Characters/Backpack/Mesh/SKM_Backpack.SKM_Backpack'"));
	if (JetpackMeshRef.Object)
	{
		Jetpack->SetSkeletalMesh(JetpackMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/StackOBot/FX/JetpackThruster/FX_JetpackThruster.FX_JetpackThruster'"));
	if (NiagaraRef.Object)
	{
		NiagaraActivationEffect = NiagaraRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Character/Input/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionChangeControlRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Character/Input/IA_ChangeControl.IA_ChangeControl'"));
	if (nullptr != InputActionChangeControlRef.Object)
	{
		ChangeControlAction = InputActionChangeControlRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Character/Input/IA_ShoulderMove.IA_ShoulderMove'"));
	if (nullptr != InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Character/Input/IA_ShoulderLook.IA_ShoulderLook'"));
	if (nullptr != InputActionShoulderLookRef.Object)
	{
		ShoulderLookAction = InputActionShoulderLookRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuaterMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Character/Input/IA_QuaterMove.IA_QuaterMove'"));
	if (nullptr != InputActionQuaterMoveRef.Object)
	{
		QuaterMoveAction = InputActionQuaterMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Character/Input/IA_Attack.IA_Attack'"));
	if (nullptr != InputActionAttackRef.Object)
	{
		AttackAction = InputActionAttackRef.Object;
	}

	static ConstructorHelpers::FClassFinder<UMainHUD> MainHUDRef(TEXT("/Game/Character/UI/WB_HUD.WB_HUD_C"));
	if (MainHUDRef.Class)
	{
		MainHUDClass = MainHUDRef.Class;
	}

	CurrentCharacterControlType = ECharacterControlType::Shoulder;
	bIsJetpackActive = false;
	bIsHovering = false;
	bCanAttack = true;
	CurrentHoveringTime = MaxHoveringTime;

}


void AMG_CharacterPlayer::BeginPlay()
{

	Super::BeginPlay();
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		EnableInput(PlayerController);
	}


	SetCharacterControl(CurrentCharacterControlType);


}

void AMG_CharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	JetPackUseTime(DeltaTime);
	
	//글리치 현상 c++로 해결하려 했으나 오류로 우선 bp처리
	/*FVector FirstMovement = FVector(1, 1, 0);
	FVector SecondMovement = FVector(-1, -1, 0);
	if (GetCharacterMovement())
		GetCharacterMovement()->MoveUpdatedComponent(FirstMovement, GetActorRotation(), true);
		GetCharacterMovement()->MoveUpdatedComponent(SecondMovement, GetActorRotation(), true);*/
		
}

void AMG_CharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMG_CharacterPlayer::StartJump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMG_CharacterPlayer::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMG_CharacterPlayer::StopJumping);
	EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &AMG_CharacterPlayer::ChangeCharacterControl);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &AMG_CharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &AMG_CharacterPlayer::ShoulderLook);
	EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &AMG_CharacterPlayer::QuaterMove);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &AMG_CharacterPlayer::Attack);

	if (IsValid(DashAction))
	{
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &AMG_CharacterPlayer::Dash);
	}

}

void AMG_CharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		EnableInput(PlayerController);
	}

	SetCharacterControl(CurrentCharacterControlType);
}

bool AMG_CharacterPlayer::IsHovering() const
{
	return bIsHovering;
}
void AMG_CharacterPlayer::ServerStartHover_Implementation()
{
	//Server에서 StartHover 상태를 모든 플레이어에게 전파
	MulticastStartHover();
}

void AMG_CharacterPlayer::MulticastStartHover_Implementation()
{
	//모든 플레이어는 특정 캐릭터의 Hover 상태를 알고, Effect 재생
	NiagaraEffect->SetAsset(NiagaraActivationEffect);
	NiagaraEffect->Activate();
	bIsHovering = true;
	GetCharacterMovement()->AirControl = 5.f;
}

void AMG_CharacterPlayer::ServerLaunchCharacter_Implementation()
{
	MulticastLaunchCharacter();
}
void AMG_CharacterPlayer::MulticastLaunchCharacter_Implementation()
{
	LaunchCharacter(FVector(0.f, 0.f, 1.f), false, true);
}

void AMG_CharacterPlayer::ServerStopHover_Implementation()
{
	//Hover 상태가 종료된 상태를 모든 플레이어에게 전파
	MulticastStopHover();
}
void AMG_CharacterPlayer::MulticastStopHover_Implementation()
{
	NiagaraEffect->Deactivate();
	bIsHovering = false;
	GetCharacterMovement()->AirControl = 1.f;
}

void AMG_CharacterPlayer::ChangeCharacterControl()
{
	if (CurrentCharacterControlType == ECharacterControlType::Shoulder)
	{
		SetCharacterControl(ECharacterControlType::Quater);
	}
	else if (CurrentCharacterControlType == ECharacterControlType::Quater)
	{
		SetCharacterControl(ECharacterControlType::Shoulder);
	}
}

void AMG_CharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	UMG_ControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);

	SetCharacterControlData(NewCharacterControl);

	if (GetController() == nullptr)
	{
		return;
	}

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}

	CurrentCharacterControlType = NewCharacterControlType;
}

void AMG_CharacterPlayer::SetCharacterControlData(const UMG_ControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	CameraArm->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraArm->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraArm->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraArm->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraArm->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraArm->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraArm->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}

void AMG_CharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AMG_CharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AMG_CharacterPlayer::QuaterMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	float InputSizeSquared = MovementVector.SquaredLength();
	float MovementVectorSize = 1.0f;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	if (MovementVectorSizeSquared > 1.0f)
	{
		MovementVector.Normalize();
		MovementVectorSizeSquared = 1.0f;
	}
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}
	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0f);
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	AddMovementInput(MoveDirection, MovementVectorSize);
}

void AMG_CharacterPlayer::Dash()
{
	if (HasAuthority())
	{
		if (bCanDash)
		{
			FVector LaunchVector = GetVelocity() * 10.f;
			LaunchVector.Z = 0.f;
			LaunchCharacter(LaunchVector, false, false);

			bCanDash = false;
			// dash timer
			GetWorldTimerManager().SetTimer(
				DashDelayTimer, 
				this, 
				&ThisClass::OnDashDelayEnd,
				DashCoolTime,
				false
			);

		}
	}
	else
	{
		if (bCanDash)
		{
			ServerDash();
		}
	}
}

void AMG_CharacterPlayer::ServerDash_Implementation()
{
	Dash();
}

void AMG_CharacterPlayer::OnDashDelayEnd()
{
	if (HasAuthority())
	{
		bCanDash = true;
	}
}

void AMG_CharacterPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME 매크로 : #include "Net/UnrealNetwork.h" 헤더파일 추가해야 사용할 수 있음
	DOREPLIFETIME(AMG_CharacterPlayer, bCanAttack);
	DOREPLIFETIME(AMG_CharacterPlayer, CurrentWalkSpeed);
	DOREPLIFETIME(AMG_CharacterPlayer, bIsShield);
	DOREPLIFETIME(AMG_CharacterPlayer, bCanDash);
}

void AMG_CharacterPlayer::Attack()
{
	if (bCanAttack)
	{
		ServerAttack();
	}

}

void AMG_CharacterPlayer::AttackHitCheck()
{
	if (HasAuthority())
	{
		MG_LOG(LogMiniGame, Log, TEXT("%s"), TEXT("Begin"));

		FHitResult HitResult;
		FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), true, this);

		const float AttackRange = 10.0f;
		const float AttackRadius = 20.0f;
		const FVector Start = GetMesh()->GetSocketLocation(TEXT("AttackSocket")) + GetActorForwardVector();
		const FVector End = Start + GetActorForwardVector() * AttackRange;

		bool HitDetected = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(AttackRadius), Params);

		if (HitDetected)
		{

		}

#if ENABLE_DRAW_DEBUG

		FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
		float CapsuleHalfHeight = AttackRange * 0.5f;
		FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

		DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);

#endif
	}
}

bool AMG_CharacterPlayer::ServerAttack_Validate()
{
	return true;
}

void AMG_CharacterPlayer::ServerAttack_Implementation()
{
	MG_LOG(LogMiniGame, Log, TEXT("%s"), TEXT("Begin"));

	MulticastAttack();
}

void AMG_CharacterPlayer::MulticastAttack_Implementation()
{
	MG_LOG(LogMiniGame, Log, TEXT("%s"), TEXT("Begin"));

	if (HasAuthority())
	{
		bCanAttack = false;
		OnRep_CanAttack();

		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]
			{
				bCanAttack = true;
				OnRep_CanAttack();
			}
		), AttackTime, false, -1.0f);
	}

	// Animation은 Server와 Client 모두 보여야함
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ActionMontage);
}

// Server에선 호출이안됨, Client에선 자동으로 호출됨
void AMG_CharacterPlayer::OnRep_CanAttack()
{
	if (!bCanAttack)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}
	else
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}

void AMG_CharacterPlayer::StartJump()
{
	if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking)
	{
		bIsJetpackActive = false;
	}

	if (bIsJetpackActive)
	{
		ServerStartHover();
		//bIsHovering = true;
		//GetCharacterMovement()->AirControl = 5.f;
	}
	bIsJetpackActive = true;
}

void AMG_CharacterPlayer::Jump()
{
	Super::Jump();

	if (bIsHovering)
	{
		ServerLaunchCharacter();
	}
}

void AMG_CharacterPlayer::StopJumping()
{
	Super::StopJumping();

	if (bIsJetpackActive)
	{
		ServerStopHover();
	}
}

void AMG_CharacterPlayer::JetPackUseTime(float DeltaTime)
{
	if (bIsHovering)
	{
        CurrentHoveringTime -= DeltaTime;
		if (CurrentHoveringTime <= 0.0f)
		{
			ServerStopHover();
		}
	}
	else if (!bIsHovering)
	{
		CurrentHoveringTime += DeltaTime;
		if (CurrentHoveringTime >= MaxHoveringTime)
		{
			CurrentHoveringTime = MaxHoveringTime;
		}
	}

	float Percent = CurrentHoveringTime / MaxHoveringTime;
	UE_LOG(LogMiniGame, Warning, TEXT("## Hovering Percent %.2f"), Percent);
	auto PC = GetController<APlayerController>();
	if (IsValid(PC))
	{
		auto HUD = PC->GetHUD<AGameHUD>();
		if (IsValid(HUD))
		{
			auto GaugeWidget = HUD->MainHUD;
			if (IsValid(GaugeWidget))
			{
				GaugeWidget->UpdateHoveringProgress(Percent);
			}
		}
	}
}



// ------------ Item Functions. -----------

void AMG_CharacterPlayer::SetCurrentWalkSpeed(float NewCurrentWalkSpeed)
{
	if (HasAuthority())
	{
		CurrentWalkSpeed = NewCurrentWalkSpeed;
		GetCharacterMovement()->MaxWalkSpeed = NewCurrentWalkSpeed;
	}
}

void AMG_CharacterPlayer::OnRep_CurrentWalkSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = CurrentWalkSpeed;
}

void AMG_CharacterPlayer::OnBoosterItem()
{
	if (HasAuthority())
	{
		// 배리어 효과 제거.
		OnBarrierEnd();

		SetCurrentWalkSpeed(BoosterSpeed);

		// net multicast boost effect?
		MultiCast_BoosterEffect(true);
		GetWorldTimerManager().SetTimer(WalkSpeedTimer, this, &ThisClass::OnBoosterEnd, BoosterDuration, false);
	}
}

void AMG_CharacterPlayer::OnBoosterEnd()
{
	if (HasAuthority())
	{
		SetCurrentWalkSpeed(700.f);
		MultiCast_BoosterEffect(false);
	}
}


void AMG_CharacterPlayer::MultiCast_BoosterEffect_Implementation(bool IsActivated)
{
	if (IsActivated)
	{
		BoosterNiagaraEffect->Activate();
	}
	else
	{
		BoosterNiagaraEffect->Deactivate();
	}
}


// 추후에 배리어에서 스피드와 지속시간을 받아와서 적용하는 것으로 변경.
void AMG_CharacterPlayer::OnBarrierOverlap()
{
	if (HasAuthority())
	{
		// 부스터 취소. 
		OnBoosterEnd();
		// 잠시 동안만...
		SetCurrentWalkSpeed(100.f);

		// net multicast boost effect?
		// 부스터를 쓴채로 배리어에 들어오면 타이머가 겹쳐버려서 부스터 end가 호출되지 않는다...
		GetWorldTimerManager().SetTimer(WalkSpeedTimer, this, &ThisClass::OnBarrierEnd, 2.f, false);
	}
}

void AMG_CharacterPlayer::OnBarrierEnd()
{
	SetCurrentWalkSpeed(700.f);

}

void AMG_CharacterPlayer::OnShield()
{
	if (HasAuthority())
	{
		bIsShield = true;
		// shield effect
		ShieldNiagaraEffect->Activate();
		GetWorldTimerManager().SetTimer(ShieldTimer, this, &ThisClass::OnShieldEnd, 2.f, false);
	}
}

void AMG_CharacterPlayer::OnShieldEnd()
{
	bIsShield = false;
	// shield effect off
	ShieldNiagaraEffect->Deactivate();
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Shield Off"));

}

void AMG_CharacterPlayer::OnRep_IsShield()
{
	if (bIsShield)
	{
		// shield effect
		ShieldNiagaraEffect->Activate();
	}
	else
	{
		// shield effect Off
		ShieldNiagaraEffect->Deactivate();
	}
}

bool AMG_CharacterPlayer::GetIsShield()
{
	return bIsShield;
}
