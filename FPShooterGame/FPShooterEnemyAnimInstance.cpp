// Fill out your copyright notice in the Description page of Project Settings.


#include "FPShooterEnemyAnimInstance.h"
#include "FPShooterEnemy.h"

UFPShooterEnemyAnimInstance::UFPShooterEnemyAnimInstance()
{

}

void UFPShooterEnemyAnimInstance::UpdateAnimationProperties(float DeltaSecond)
{
	if (FPShooterEnemy == nullptr)
	{
		FPShooterEnemy = Cast<AFPShooterEnemy>(TryGetPawnOwner());
	}

	if (FPShooterEnemy)
	{
		FVector EnemyVelocity = FPShooterEnemy->GetVelocity();
		EnemyVelocity.Z = 0;
		MoveSpeed = EnemyVelocity.Size();
	}
}
