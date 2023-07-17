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

		// 영웅의 질주 여부를 체크한다.
		bIsSprint = FPShooterHero->GetSprintStatus();

		// 영웅의 ADS 여부를 체크한다.
		bIsADS = FPShooterHero->GetADSStatus();

		// 영웅의 Crouch 여부를 체크한다.
		bIsCrouch = FPShooterHero->GetCharacterMovement()->IsCrouching();

		// 영웅이 장착한 무기의 타입을 체크한다.
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
