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
	// 영웅 접근자
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	class AFPShooterEnemy* FPShooterEnemy;

	// 이동하는 속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed;

protected:


public:
	//매 프레임 마다 애니메이션 속성 업데이트(블루프린트에서 호출 가능)
	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaSecond);

};
