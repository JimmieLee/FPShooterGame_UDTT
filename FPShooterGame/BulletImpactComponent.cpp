// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletImpactComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstance.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

// Sets default values for this component's properties
UBulletImpactComponent::UBulletImpactComponent() :
	DecalSize(FVector(0.0f, 0.0f, 0.0f))
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UBulletImpactComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UBulletImpactComponent::PlayImpactEffects(FHitResult ImpactHit)
{
	if (ImpactHit.PhysMaterial == nullptr)
	{
		/* DEBUG_LINE */
		UE_LOG(LogTemp, Warning, TEXT("No Physics Materials"));
		return;
	}

	const EPhysicalSurface Surface = ImpactHit.PhysMaterial->SurfaceType;

	// Hit된 지점의 위치와 Normal 회전을 구해서 데칼을 생성.
	const FVector DecalLocation = ImpactHit.ImpactPoint;
	const FRotator DecalRotation = ImpactHit.ImpactNormal.Rotation();

	switch (Surface)
	{
	case SurfaceType1:		
		// Hit된 오브젝트의 Physical Material이 Concrete일 경우 SF & SFX 재생 및 데칼 생성.
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactContrete, ImpactHit.ImpactPoint);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactContreteSound, ImpactHit.ImpactPoint);				
		UGameplayStatics::SpawnDecalAtLocation(GetWorld(), ImapactConcreteDecal, DecalSize, DecalLocation, DecalRotation);
		break;

	case SurfaceType2:
		// Hit된 오브젝트의 Physical Material이 Metal일 경우 SF & SFX 재생
		/* DEBUG_LINE */
		UE_LOG(LogTemp, Warning, TEXT("Metal Hit!!"));

		break;

	case SurfaceType3:
		// Hit된 오브젝트의 Physical Material이 Flesh일 경우 SF & SFX 재생
		/* DEBUG_LINE */
		UE_LOG(LogTemp, Warning, TEXT("Flesh Hit!!"));

		break;
	}
}

// Called every frame
void UBulletImpactComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

