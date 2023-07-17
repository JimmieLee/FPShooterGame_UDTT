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
	// ���� ������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hero", meta = (AllowPrivateAccess = "true"))
	class AFPShooterHero* FPShooterHero;

	// �̵� �ӵ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed;

	// ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	// ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;

	// ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsSprint;

	// ADS ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Compbat", meta = (AllowPrivateAccess = "true"))
	bool bIsADS;

	// Crouch ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Compbat", meta = (AllowPrivateAccess = "true"))
	bool bIsCrouch;

	// ������ ������ Ÿ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Compbat", meta = (AllowPrivateAccess = "true"))
	EWeaponType WeaponType;

protected:	

public:
	//�� ������ ���� �ִϸ��̼� �Ӽ� ������Ʈ(�������Ʈ���� ȣ�� ����)
	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaSecond);

	virtual void NativeInitializeAnimation() override;

};
