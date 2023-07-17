// Fill out your copyright notice in the Description page of Project Settings.


#include "FPShooterAnimInstance.h"
#include "FPShooterHero.h"
#include "FPShooterWeapon.h"
#include "GameFramework/CharacterMovementComponent.h"

UFPShooterAnimInstance::UFPShooterAnimInstance() :
	MoveSpeed{ 0.0f },
	bIsInAir { false },
	bIsAccelerating { false },
	bIsADS { false }
{

}

void UFPShooterAnimInstance::UpdateAnimationProperties(float DeltaSecond)
{
	if (FPShooterHero == nullptr)
	{
		FPShooterHero = Cast<AFPShooterHero>(TryGetPawnOwner());
	}

	if (FPShooterHero)
	{
		FVector HeroVelocity = FPShooterHero->GetVelocity();
		HeroVelocity.Z = 0;
		MoveSpeed = HeroVelocity.Size();

		bIsInAir = FPShooterHero->GetCharacterMovement()->IsFalling();

		if (FPShooterHero->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0f)
		{
			bIsAccelerating = true;
		}
		else
		{
			bIsAccelerating = false;
		}

		// ������ ���� ���θ� üũ�Ѵ�.
		bIsSprint = FPShooterHero->GetSprintStatus();

		// ������ ADS ���θ� üũ�Ѵ�.
		bIsADS = FPShooterHero->GetADSStatus();

		// ������ Crouch ���θ� üũ�Ѵ�.
		bIsCrouch = FPShooterHero->GetCharacterMovement()->IsCrouching();

		// ������ ������ ������ Ÿ���� üũ�Ѵ�.
		if (FPShooterHero->GetEquippedWeapon())
		{
			WeaponType = FPShooterHero->GetEquippedWeapon()->GetWeaponType();
		}		
	}	
}

void UFPShooterAnimInstance::NativeInitializeAnimation()
{
	FPShooterHero = Cast<AFPShooterHero>(TryGetPawnOwner());
}
