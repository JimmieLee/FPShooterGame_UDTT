// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FPShooterHitInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFPShooterHitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FPSHOOTERGAME_API IFPShooterHitInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// ���� ������ ���� �ǰ� ó��
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TraceHit(FHitResult TraceHit);

	// �߻�ü �浹�� ���� �ǰ� ó��
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ProjectileHit();
};
