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
	// ���� HP
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Life", meta = (AllowPrivateAccess = "true"))
	float CurrentHealth;

	// �ִ� HP
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LIfe", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	// �� ������
	float ArmorDurability;

	// �ִ� �� ������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Armor", meta = (AllowPrivateAccess = "true"))
	float MaxArmorDurability;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// �� ������ ���� üũ.
	float CheckArmorState();

	// ����� ���� ó��.
	void ApplyHealthDamage(float DamageAmount);

	// �� ���� ó��.
	void ApplyArmorDamage(float DamageAmount);	

public:	
	// private ���� Getter
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth;  }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	// ���� ó��.
	void TakeDamage(float DamageAmount);	

	// ����� ���� ����.
	void Heal(float HealAmount);

	// �� ���� ����.
	void RepairArmor(float ArmorAmount);

	// % ���
	float AsPercentage(float A, float B);
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;	
};
