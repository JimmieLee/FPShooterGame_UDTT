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

	// 무기가 보유한 총알은 탄창의 크기와 동일.
	CurrentAmmoAmount = MagazineCapacity;
	
	MuzzleSocket = GetItemMesh()->GetSocketByName(MuzzleSocketName);
}

void AFPShooterWeapon::ConsumeAmmo(int32 Amount)
{
	// 보유 중인 총알을 1개 감소.
	CurrentAmmoAmount -= Amount;
	
	/*DEBUG_LINE*/
	UE_LOG(LogTemp, Warning, TEXT("Current Ammo Amount is: %d"), CurrentAmmoAmount);
}

int32 AFPShooterWeapon::GetEmptyMagazineCapacity()
{
	/*DEBUG_LINE*/
	UE_LOG(LogTemp, Warning, TEXT("Empty Magazine Capacity is: %d"), MagazineCapacity - CurrentAmmoAmount);

	// 탄창의 비어있는 총알의 개수를 전달.
	return MagazineCapacity - CurrentAmmoAmount;	
}

void AFPShooterWeapon::ChargeAmmo(int32 Amount)
{
	// 전달된 총알의 개수를 보유 중인 총알에 합산
	CurrentAmmoAmount += Amount;

	/*DEBUG_LINE*/
	UE_LOG(LogTemp, Warning, TEXT("Charge Ammo Amount is: %d"), Amount);

	// 보유 중인 총알의 개수가 탄창 크기보다 크면, 탄창의 크기만큼으로 설정.
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



