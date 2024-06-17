// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_AnimInstance.h"
#include "Minigames/OBot/Character/MG_CharacterPlayer.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UMG_AnimInstance::UMG_AnimInstance()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;
}

void UMG_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	// AnimInstance를 사용하는 Actor를 가져올수잇음
	Owner = Cast<AMG_CharacterPlayer>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UMG_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshould;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);
		bIsHovering = Owner->IsHovering();
		bIsHitAttack = Owner->IsHitAttack();
	}

}
