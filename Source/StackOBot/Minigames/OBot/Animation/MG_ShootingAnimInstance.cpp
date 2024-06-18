// Fill out your copyright notice in the Description page of Project Settings.


#include "MG_ShootingAnimInstance.h"
#include "Minigames/OBot/Character/MG_ShootingCharacterPlayer.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UMG_ShootingAnimInstance::UMG_ShootingAnimInstance()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;
}

void UMG_ShootingAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Owner = Cast<AMG_ShootingCharacterPlayer>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UMG_ShootingAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshould;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);
		bIsCrouching = Movement->IsCrouching();

		FVector VelocityUnitVector = Velocity.GetSafeNormal2D();
		FVector ForwardVector = Owner->GetActorForwardVector();
		Owner->GetActorUpVector().Z;
		float Angle = FMath::Acos(FVector::DotProduct(VelocityUnitVector, ForwardVector));
		Angle *= (180.f / PI);
		if (FVector::CrossProduct(VelocityUnitVector, ForwardVector).Z * Owner->GetActorUpVector().Z >= 0)
		{
			Dir = -Angle;
		}
		else
		{
			Dir = Angle;
		}
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Dir %f"), Dir));
	}
}
