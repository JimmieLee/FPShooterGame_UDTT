// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPShooterProjectile.generated.h"

UCLASS()
class FPSHOOTERGAME_API AFPShooterProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPShooterProjectile();

private:
	// �߻�ü �޽� ������Ʈ.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ProjectileMesh;

	// �߻�ü � ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovment;

	// �߻� ���� ȿ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Effect", meta = (AllowPrivateAccess = "true"))
	class UNiagaraSystem* ProjecileTrail;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// �߻�ü � ������Ʈ�� ���� �ܺ� ���� �Լ�.
	FORCEINLINE class UStaticMeshComponent* GetProjectileMesh() const { return ProjectileMesh; }

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
