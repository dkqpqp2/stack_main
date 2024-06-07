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
	

	CurrentCharacterControlType = ECharacterControlType::Shoulder;
	bIsJetpackActive = false;
	bCanAttack = true;

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

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &AMG_CharacterPlayer::ServerStopHover, HoveringTime, false);
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

void AMG_CharacterPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME 매크로 : #include "Net/UnrealNetwork.h" 헤더파일 추가해야 사용할 수 있음
	DOREPLIFETIME(AMG_CharacterPlayer, bCanAttack);
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

void AMG_CharacterPlayer::OnBoosterItem()
{
	// 잠시 동안만...
	GetCharacterMovement()->MaxWalkSpeed = 1500.f;

	// net multicast boost effect?
	GetWorldTimerManager().SetTimer(Timer, this, &ThisClass::OnBoosterEnd, 3.f, false);
	
}

void AMG_CharacterPlayer::OnRep_Hover()
{
	//부스터 게이지로 전환 함수 
}

void AMG_CharacterPlayer::OnBoosterEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = 700.f;

}

void AMG_CharacterPlayer::OnBarrierOverlap()
{
	GetCharacterMovement()->MaxWalkSpeed = 100.f;

	GetWorldTimerManager().SetTimer(Timer, this, &ThisClass::OnBarrierEnd, 1.f, false);
}

void AMG_CharacterPlayer::OnBarrierEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = 700.f;

}
