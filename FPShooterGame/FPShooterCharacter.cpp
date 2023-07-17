// Fill out your copyright notice in the Description page of Project Settings.


#include "FPShooterCharacter.h"
#include "FPShooterHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimMontage.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"


// Sets default values
AFPShooterCharacter::AFPShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UFPShooterHealthComponent>(TEXT("Health Manager"));
}

// Called when the game starts or when spawned
void AFPShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


