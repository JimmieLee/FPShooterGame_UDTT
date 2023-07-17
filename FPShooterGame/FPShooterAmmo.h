// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPShooterItem.h"
#include "WeaponProperties.h"
#include "FPShooterAmmo.generated.h"

/**
 * 
 */
UCLASS()
class FPSHOOTERGAME_API AFPShooterAmmo : public AFPShooterItem
{
	GENERATED_BODY()

public:
	AFPShooterAmmo();
	
private:
	// 탄약의 외형
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* AmmoMesh;

	// 탄약 타입
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	EAmmoType AmmoType;

protected:

public:
	// 탄약 타입에 대한 외부 접근.
	FORCEINLINE EAmmoType GetAmmoType() const { return AmmoType; }
};
