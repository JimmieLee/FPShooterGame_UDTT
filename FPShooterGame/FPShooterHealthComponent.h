// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FPShooterHealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSHOOTERGAME_API UFPShooterHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFPShooterHealthComponent();

private:
	// 현재 HP
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Life", meta = (AllowPrivateAccess = "true"))
	float CurrentHealth;

	// 최대 HP
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LIfe", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	// 방어구 내구도
	float ArmorDurability;

	// 최대 방어구 내구도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Armor", meta = (AllowPrivateAccess = "true"))
	float MaxArmorDurability;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// 방어구 내구도 상태 체크.
	float CheckArmorState();

	// 생명력 피해 처리.
	void ApplyHealthDamage(float DamageAmount);

	// 방어구 피해 처리.
	void ApplyArmorDamage(float DamageAmount);	

public:	
	// private 변수 Getter
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth;  }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	// 피해 처리.
	void TakeDamage(float DamageAmount);	

	// 생명력 피해 복구.
	void Heal(float HealAmount);

	// 방어구 피해 복구.
	void RepairArmor(float ArmorAmount);

	// % 계산
	float AsPercentage(float A, float B);
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;	
};
