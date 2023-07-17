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

	// Hit�� ������ ��ġ�� Normal ȸ���� ���ؼ� ��Į�� ����.
	const FVector DecalLocation = ImpactHit.ImpactPoint;
	const FRotator DecalRotation = ImpactHit.ImpactNormal.Rotation();

	switch (Surface)
	{
	case SurfaceType1:		
		// Hit�� ������Ʈ�� Physical Material�� Concrete�� ��� SF & SFX ��� �� ��Į ����.
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactContrete, ImpactHit.ImpactPoint);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactContreteSound, ImpactHit.ImpactPoint);				
		UGameplayStatics::SpawnDecalAtLocation(GetWorld(), ImapactConcreteDecal, DecalSize, DecalLocation, DecalRotation);
		break;

	case SurfaceType2:
		// Hit�� ������Ʈ�� Physical Material�� Metal�� ��� SF & SFX ���
		/* DEBUG_LINE */
		UE_LOG(LogTemp, Warning, TEXT("Metal Hit!!"));

		break;

	case SurfaceType3:
		// Hit�� ������Ʈ�� Physical Material�� Flesh�� ��� SF & SFX ���
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

