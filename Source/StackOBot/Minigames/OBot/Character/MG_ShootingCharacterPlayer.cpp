// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_ShootingCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "../../../ShootingGames/Weapons/WeaponBase.h"	
#include "../../../ShootingGames/FPSHUD.h"	
#include "../../../ShootingGames/FPSPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

AMG_ShootingCharacterPlayer::AMG_ShootingCharacterPlayer()
{
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->TargetArmLength = 400.f;
	CameraArm->bUsePawnControlRotation = true;

	CameraArm->bEnableCameraLag = true;
	CameraArm->bEnableCameraRotationLag = true;
	CameraArm->CameraLagSpeed = 20.0f;
	CameraArm->CameraRotationLagSpeed = 20.0f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	Jetpack = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Jetpack"));
	Jetpack->SetupAttachment(GetMesh(), FName("BackpackSocket"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> JetpackMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/StackOBot/Characters/Backpack/Mesh/SKM_Backpack.SKM_Backpack'"));
	if (JetpackMeshRef.Object)
	{
		Jetpack->SetSkeletalMesh(JetpackMeshRef.Object);
	}



}

void AMG_ShootingCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	AFPSPlayerController* PlayerController = Cast<AFPSPlayerController>(GetController());
	if (PlayerController)
	{
		EnableInput(PlayerController);
		//PlayerController->SetHUDHealth(CurrentHealth, MaxHealth);
		HUD = Cast<AFPSHUD>(PlayerController->GetHUD());
	}
	UpdateHUDHealth();
	if (HasAuthority())
	{
		OnTakeAnyDamage.AddDynamic(this, &AMG_ShootingCharacterPlayer::ReceiveDamage);
	}
	
}

void AMG_ShootingCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetHUDCrossHair(DeltaTime);
}


void AMG_ShootingCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMG_ShootingCharacterPlayer::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMG_ShootingCharacterPlayer::Look);
	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AMG_ShootingCharacterPlayer::OnCrouch);
	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AMG_ShootingCharacterPlayer::OffCrouch);
	// Test
}

void AMG_ShootingCharacterPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMG_ShootingCharacterPlayer, HasWeapon);
	DOREPLIFETIME(AMG_ShootingCharacterPlayer, CurrentWeaponBase);
	DOREPLIFETIME(AMG_ShootingCharacterPlayer, RifleAmmos);
	DOREPLIFETIME(AMG_ShootingCharacterPlayer, PistolAmmos);
	DOREPLIFETIME(AMG_ShootingCharacterPlayer, GrenadeAmmos);
	DOREPLIFETIME(AMG_ShootingCharacterPlayer, CurrentHealth);
	DOREPLIFETIME(AMG_ShootingCharacterPlayer, IsReloading);
}

void AMG_ShootingCharacterPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AMG_ShootingCharacterPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(-LookAxisVector.Y);
}

void AMG_ShootingCharacterPlayer::OnCrouch(const FInputActionValue& Value)
{
	if (!GetCharacterMovement()->IsFalling())
	{
		Crouch(false);
	}
}

void AMG_ShootingCharacterPlayer::OffCrouch(const FInputActionValue& Value)
{
	UnCrouch(false);
}


void AMG_ShootingCharacterPlayer::SetHUDCrossHair(float DeltaTime)
{
	if (HUD)
	{
		FHUDPackage HUDPackage;
		//무기를 끼고 있으면 
		if (IsValid(CurrentWeaponBase))
		{

			HUDPackage.CrosshairsCenter = CurrentWeaponBase->CrosshairsCenter;
			HUDPackage.CrosshairsLeft = CurrentWeaponBase->CrosshairsLeft;
			HUDPackage.CrosshairsRight = CurrentWeaponBase->CrosshairsRight;
			HUDPackage.CrosshairsTop = CurrentWeaponBase->CrosshairsTop;
			HUDPackage.CrosshairsBottom = CurrentWeaponBase->CrosshairsBottom;
		}
		else
		{
			HUDPackage.CrosshairsCenter = nullptr;
			HUDPackage.CrosshairsLeft = nullptr;
			HUDPackage.CrosshairsRight = nullptr;
			HUDPackage.CrosshairsBottom = nullptr;
			HUDPackage.CrosshairsTop = nullptr;
		}
		//Calculate Crosshair spread
		//[0,600] -> [0,1]
		FVector2D WalkSpeedRange(0.f, GetCharacterMovement()->MaxWalkSpeed);
		FVector2D VelocityMultiplierRange(0.f, 1.f);//charactermovement를 0-1로 정규화
		FVector Velocity = GetVelocity();
		Velocity.Z = 0.f;

		CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, Velocity.Size());
		//속도를 기반으로 퍼지는걸 가져오는 함수 
		CrosshairShootingFactor =FMath::FInterpTo(CrosshairShootingFactor, 0.f, DeltaTime, 40.f);
		HUDPackage.CrosshairSpread = CrosshairVelocityFactor + CrosshairShootingFactor;
		HUD->SetHUDPackage(HUDPackage);
	}

}

void AMG_ShootingCharacterPlayer::OnRep_Health()
{
	GetWorld()->GetFirstPlayerController()->GetPawn<AMG_ShootingCharacterPlayer>()->UpdateHUDHealth();
	//UpdateHUDHealth();
	// 애니메이션 보여도 됨 
	//체력이랑 애니메이션은 서버에서도 보여아 함으로 여기서 처리 해도 괜찮음
}

void AMG_ShootingCharacterPlayer::ReceiveDamage(AActor* DamagaedActor, float Damagae, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damagae, 0.f, MaxHealth); // 체력을 0-100 제한
	GetWorld()->GetFirstPlayerController()->GetPawn<AMG_ShootingCharacterPlayer>()->UpdateHUDHealth();

	if (CurrentHealth <= 0.f)
	{
		OnDeathPlayRagdoll();
	}

	//UpdateHUDHealth();
	// 맞을때 애니메이션 넣어도 됨
	// 
	//AGameState* GS = GetWorld()->GetGameState<AGameState>();
	//if (!IsValid(GS))
	//{
	//	return;
	//}

	//for (APlayerState* PS : GS->PlayerArray)
	//{
	//	AMG_ShootingCharacterPlayer* ShootingPlayer = PS->GetPawn<AMG_ShootingCharacterPlayer>();
	//	if (!IsValid(ShootingPlayer))
	//	{
	//		return;
	//	}
	//	
	//	// netmulticast 함수 호출.
	//	ShootingPlayer->Multicast_RefreshHP();

	//}
}

void AMG_ShootingCharacterPlayer::UpdateHUDHealth()
{
	FPSPlayerController = FPSPlayerController == nullptr ? Cast<AFPSPlayerController>(Controller) : FPSPlayerController;
	if (FPSPlayerController)
	{
		FPSPlayerController->SetHUDHealth(CurrentHealth, MaxHealth, PlayerName);
	}
	if (!FPSPlayerController)
	{
		return;
	}
	//컨트롤러가 아니라면 ? Client 2-4  적용할 위젯은 
	AGameState* GS = GetWorld()->GetGameState<AGameState>();
	if (!IsValid(GS))
	{
		return;
	}
	int i = 1;
	for (APlayerState* PS : GS->PlayerArray)//모든 플레이어 정보를 가져오는 state를 참조하고
	{
		if (GetPlayerState() == PS)
		{
			continue;
		}
		AMG_ShootingCharacterPlayer* ShootingPlayer = PS->GetPawn<AMG_ShootingCharacterPlayer>();
		if (!IsValid(ShootingPlayer))
		{
			return;
		}
		float PlayerHealth = ShootingPlayer->CurrentHealth;
		FString PlayerSteamName = PS->GetPlayerName();
		if (i == 1)
		{
			FPSPlayerController->SetHUDHealth_1(PlayerHealth, MaxHealth,PlayerSteamName);

		}
		else if (i == 2)
		{
			FPSPlayerController->SetHUDHealth_2(PlayerHealth, MaxHealth,PlayerSteamName);

		}
		else if (i == 3)
		{
			FPSPlayerController->SetHUDHealth_3(PlayerHealth, MaxHealth,PlayerSteamName);

		}
		i += 1;
		

	}
}

void AMG_ShootingCharacterPlayer::OnDeathPlayRagdoll()
{
	if (HasAuthority())
	{
		Multicast_PlayRagdoll();
	}

}

void AMG_ShootingCharacterPlayer::OnDeathEnd()
{
	GetMesh()->SetAllBodiesSimulatePhysics(false);
	GetCharacterMovement()->SetDefaultMovementMode();
	GetMesh()->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	Jetpack->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("BackpackSocket"));
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -60), FRotator(0, -90, 0));
	if (HasAuthority())
	{
		//AActor* PlayerStart = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());
		AActor* PlayerStart = GetWorld()->GetAuthGameMode()->FindPlayerStart(GetController(), FString::FromInt(1));
		//다른 물체가 닿으면 start 위치로 location 설정
		if (IsValid(PlayerStart))
		{
			SetActorLocation(PlayerStart->GetActorLocation());
			CurrentHealth = MaxHealth;
			GetWorld()->GetFirstPlayerController()->GetPawn<AMG_ShootingCharacterPlayer>()->UpdateHUDHealth();
			//GetMesh()->AttachToComponent()
			//Jetpack->SetupAttachment(GetMesh(), FName("BackpackSocket"));
		}
	}

	
}

void AMG_ShootingCharacterPlayer::Multicast_PlayRagdoll_Implementation()
{
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetCharacterMovement()->DisableMovement();
	// restart function...
	GetWorldTimerManager().SetTimer(DeathTimer, this, &ThisClass::OnDeathEnd, 2.f, false);
}




void AMG_ShootingCharacterPlayer::TryReloadWeapon()
{
	if (!HasAuthority())
	{
		return;
	}
	if (!IsValid(CurrentWeaponBase))
	{
		return;
	}
	int32 CurrentPlayerAmmos = 0;
	switch (CurrentWeaponBase->WeaponType)
	{
	case EWeaponType::Rifle:
		CurrentPlayerAmmos = RifleAmmos;
		break;
	case EWeaponType::Pistol:
		CurrentPlayerAmmos = PistolAmmos;
		break;
	case EWeaponType::Grenade:
		CurrentPlayerAmmos = GrenadeAmmos;
		break;
	default:
		break;
	}

	if (CurrentWeaponBase->MaxAmmo - CurrentWeaponBase->CurrentAmmo > 0
		&& CurrentPlayerAmmos > 0 )
	{
		PlayReloadStartAnimation();
		IsReloading = true;
	}
}

void AMG_ShootingCharacterPlayer::OnEndReloadAnimation()
{
	if (!HasAuthority())
	{
		return;
	}

	if (!IsValid(CurrentWeaponBase))
	{
		return;
	}
	IsReloading = false;

	int32 CurrentPlayerAmmos = 0;
	switch (CurrentWeaponBase->WeaponType)
	{
	case EWeaponType::Rifle:
		CurrentPlayerAmmos = RifleAmmos;
		break;
	case EWeaponType::Pistol:
		CurrentPlayerAmmos = PistolAmmos;
		break;
	case EWeaponType::Grenade:
		CurrentPlayerAmmos = GrenadeAmmos;
		break;
	default:
		break;
	}

	int32 NeedToFillAmmos = CurrentWeaponBase->MaxAmmo - CurrentWeaponBase->CurrentAmmo;
	if (CurrentPlayerAmmos >= NeedToFillAmmos)
	{
		CurrentWeaponBase->CurrentAmmo = CurrentWeaponBase->MaxAmmo;
		CurrentPlayerAmmos -= NeedToFillAmmos;
	}
	else
	{
		CurrentWeaponBase->CurrentAmmo += CurrentPlayerAmmos;
		CurrentPlayerAmmos = 0;
	}

	switch (CurrentWeaponBase->WeaponType)
	{
	case EWeaponType::Rifle:
		RifleAmmos = CurrentPlayerAmmos;
		break;
	case EWeaponType::Pistol:
		PistolAmmos = CurrentPlayerAmmos;
		break;
	case EWeaponType::Grenade:
		GrenadeAmmos = CurrentPlayerAmmos;
		break;
	default:
		break;
	}
	CurrentWeaponBase->SetHUDBullet();
}

void AMG_ShootingCharacterPlayer::PlayReloadStartAnimation_Implementation()
{
	if (IsValid(ReloadAnimMontage))
	{
		PlayAnimMontage(ReloadAnimMontage);
	}
}

void AMG_ShootingCharacterPlayer::Multicast_RefreshHP_Implementation()
{
	UpdateHUDHealth();
}



