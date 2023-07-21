// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FPShooterEnemyAnimInstance.generated.h"

UCLASS()
class FPSHOOTERGAME_API UFPShooterEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFPShooterEnemyAnimInstance();

private:
	// ���� ������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	class AFPShooterEnemy* FPShooterEnemy;

	// �̵��ϴ� �ӵ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed;

protected:


public:
	//�� ������ ���� �ִϸ��̼� �Ӽ� ������Ʈ(�������Ʈ���� ȣ�� ����)
	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaSecond);

};
