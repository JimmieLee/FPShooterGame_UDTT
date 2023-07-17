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
	// 발사체 메시 컴포넌트.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ProjectileMesh;

	// 발사체 운동 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovment;

	// 발사 궤적 효과
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Effect", meta = (AllowPrivateAccess = "true"))
	class UNiagaraSystem* ProjecileTrail;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// 발사체 운동 컴포넌트에 대한 외부 접근 함수.
	FORCEINLINE class UStaticMeshComponent* GetProjectileMesh() const { return ProjectileMesh; }

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
