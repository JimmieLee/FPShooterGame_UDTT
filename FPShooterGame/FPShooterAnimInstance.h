// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WeaponProperties.h"
#include "FPShooterAnimInstance.generated.h"

UCLASS()
class FPSHOOTERGAME_API UFPShooterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UFPShooterAnimInstance();

private:
	// 영웅 접근자
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hero", meta = (AllowPrivateAccess = "true"))
	class AFPShooterHero* FPShooterHero;

	// 이동 속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed;

	// 점프 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	// 가속 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;

	// 질주 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsSprint;

	// ADS 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Compbat", meta = (AllowPrivateAccess = "true"))
	bool bIsADS;

	// Crouch 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Compbat", meta = (AllowPrivateAccess = "true"))
	bool bIsCrouch;

	// 장착된 무기의 타입
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Compbat", meta = (AllowPrivateAccess = "true"))
	EWeaponType WeaponType;

protected:	

public:
	//매 프레임 마다 애니메이션 속성 업데이트(블루프린트에서 호출 가능)
	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaSecond);

	virtual void NativeInitializeAnimation() override;

};
