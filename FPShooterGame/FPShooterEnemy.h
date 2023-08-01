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
	// Bullet Impact ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBulletImpactComponent* BulletImpact;

	// Health Widget�� 3D ȯ�� �� ǥ���� �� �ִ� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* HealthWidgetComp;

	// Health Widget Ŭ����
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

	// ���
	void Death();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ���� ���� �浹�� ���� �ǰ� ó�� �Լ� (IFPShooterHitInterface���� override)
	void TraceHit_Implementation(FHitResult TraceHit) override;

	// �߻�ü �浹�� ���� �ǰ� ó�� �ռ� (IFPShooterHitInterface���� override)

};
