// Fill out your copyright notice in the Description page of Project Settings.


#include "FPShooterProjectile.h"

// Sets default values
AFPShooterProjectile::AFPShooterProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Body"));
}

// Called when the game starts or when spawned
void AFPShooterProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPShooterProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

