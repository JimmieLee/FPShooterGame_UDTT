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
	// ź���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* AmmoMesh;

	// ź�� Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	EAmmoType AmmoType;

protected:

public:
	// ź�� Ÿ�Կ� ���� �ܺ� ����.
	FORCEINLINE EAmmoType GetAmmoType() const { return AmmoType; }
};
