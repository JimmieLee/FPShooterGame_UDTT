// Fill out your copyright notice in the Description page of Project Settings.


#include "FPShooterItemSpawner.h"

// Sets default values
AFPShooterItemSpawner::AFPShooterItemSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFPShooterItemSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPShooterItemSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

