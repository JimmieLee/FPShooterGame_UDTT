// Fill out your copyright notice in the Description page of Project Settings.


#include "FPShooterWeapon.h"
#include "FPShooterProjectile.h"

AFPShooterWeapon::AFPShooterWeapon() :
	Distance(500.0f),
	MagazineCapacity(12),
	WeaponRecoil(0.0f),
	WeaponRecoilSpread(0.0f, 0.0f)
{

}

void AFPShooterWeapon::BeginPlay()
{
	Super::BeginPlay();

	// ���Ⱑ ������ �Ѿ��� źâ�� ũ��� ����.
	CurrentAmmoAmount = MagazineCapacity;
	
	MuzzleSocket = GetItemMesh()->GetSocketByName(MuzzleSocketName);
}

void AFPShooterWeapon::ConsumeAmmo(int32 Amount)
{
	// ���� ���� �Ѿ��� 1�� ����.
	CurrentAmmoAmount -= Amount;
	
	/*DEBUG_LINE*/
	UE_LOG(LogTemp, Warning, TEXT("Current Ammo Amount is: %d"), CurrentAmmoAmount);
}

int32 AFPShooterWeapon::GetEmptyMagazineCapacity()
{
	/*DEBUG_LINE*/
	UE_LOG(LogTemp, Warning, TEXT("Empty Magazine Capacity is: %d"), MagazineCapacity - CurrentAmmoAmount);

	// źâ�� ����ִ� �Ѿ��� ������ ����.
	return MagazineCapacity - CurrentAmmoAmount;	
}

void AFPShooterWeapon::ChargeAmmo(int32 Amount)
{
	// ���޵� �Ѿ��� ������ ���� ���� �Ѿ˿� �ջ�
	CurrentAmmoAmount += Amount;

	/*DEBUG_LINE*/
	UE_LOG(LogTemp, Warning, TEXT("Charge Ammo Amount is: %d"), Amount);

	// ���� ���� �Ѿ��� ������ źâ ũ�⺸�� ũ��, źâ�� ũ�⸸ŭ���� ����.
	if (CurrentAmmoAmount > MagazineCapacity)
	{
		CurrentAmmoAmount = MagazineCapacity;
	}

	/*DEBUG_LINE*/
	UE_LOG(LogTemp, Warning, TEXT("Current Ammo Amount is: %d"), CurrentAmmoAmount);
}

AFPShooterProjectile* AFPShooterWeapon::SpawnProjectile(FVector Location, FRotator Rotation)
{
	if (ProjectileClass)
	{
		AFPShooterProjectile* SpawnedProjectile;

		SpawnedProjectile = GetWorld()->SpawnActor<AFPShooterProjectile>(ProjectileClass, Location, Rotation);

		if (SpawnedProjectile)
		{	
			return SpawnedProjectile;
		}
	}
	return nullptr;
}

void AFPShooterWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
}



