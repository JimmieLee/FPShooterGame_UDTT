// Fill out your copyright notice in the Description page of Project Settings.


#include "FPShooterAmmo.h"
#include "Components/StaticMeshComponent.h"

AFPShooterAmmo::AFPShooterAmmo() :
	AmmoType (EAmmoType::EAT_Pistol)
{
	AmmoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ammo Body"));
	if (AmmoMesh)
	{
		AmmoMesh->SetupAttachment(RootComponent);
	}
}


