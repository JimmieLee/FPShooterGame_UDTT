// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPShooterCharacter.h"
#include "FPShooterHitInterface.h"
#include "FPShooterEnemy.generated.h"

UCLASS()
class FPSHOOTERGAME_API AFPShooterEnemy : public AFPShooterCharacter, public IFPShooterHitInterface
{
	GENERATED_BODY()
	

public:
	AFPShooterEnemy();

private:
	// Bullet Impact 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBulletImpactComponent* BulletImpact;

	// Health Widget을 3D 환경 상에 표시할 수 있는 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* HealthWidgetComp;

	// Health Widget 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> HealthWidgetClass;

	// Health Widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UUserWidget* HealthWidget;

	// Health Point Percentage
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	float HealthPointPercentage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 사망
	void Death();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 라인 추적 충돌에 의한 피격 처리 함수 (IFPShooterHitInterface에서 override)
	void TraceHit_Implementation(FHitResult TraceHit) override;

	// 발사체 충돌에 의한 피격 처리 합수 (IFPShooterHitInterface에서 override)

};
