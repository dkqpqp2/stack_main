// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_ShootingCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Minigames/GameMap/GamePlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMG_ShootingCharacterBase::AMG_ShootingCharacterBase()
{
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCapsuleComponent()->InitCapsuleSize(35.0f, 60.0f);
    GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));

    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 700.0f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->SetWalkableFloorAngle(50.0f);
    GetCharacterMovement()->MaxWalkSpeed = 500.0f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
    GetCharacterMovement()->GravityScale = 1.0f;
    GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

    GetMesh()->SetRelativeLocationAndRotation(FVector(0.0, 0.0, -60.0), FRotator(0.0, -90.0, 0.0));
    GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/StackOBot/Characters/Bot/Mesh/SKM_Bot.SKM_Bot'"));
    if (CharacterMeshRef.Object)
    {
        GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);

    }

    //static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Character/Animation/ShootingPlayer/MG_ShootingOBot.MG_ShootingOBot_C"));
    //if (AnimInstanceClassRef.Class)
    //{
    //    GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
    //}

}

// Called when the game starts or when spawned
void AMG_ShootingCharacterBase::BeginPlay()
{
	Super::BeginPlay();
    MaterialDynamicInstance = GetMesh()->CreateDynamicMaterialInstance(0);

    auto PS = GetPlayerState<AGamePlayerState>();
    if (IsValid(PS))
    {
        //GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Magenta, TEXT("Set Material TeamColor To Bot At BeginPlay()"));
        SetMaterialToTeamColor(PS->GetPlayerColor());
    }
}

void AMG_ShootingCharacterBase::SetMaterialToTeamColor(FVector4 NewColor)
{
    if (!IsValid(MaterialDynamicInstance))
    {
        //GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("DYNAMIC MATERIAL NOT VALID : AMG_CharacterBase::SetMaterialToTeamColor"));
        return;
    }
    MaterialDynamicInstance->SetVectorParameterValue(FName("MainColor"), NewColor);

}


