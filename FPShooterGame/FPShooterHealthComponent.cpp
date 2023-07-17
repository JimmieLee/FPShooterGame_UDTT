// Fill out your copyright notice in the Description page of Project Settings.


#include "FPShooterHealthComponent.h"

// Sets default values for this component's properties
UFPShooterHealthComponent::UFPShooterHealthComponent() :
	CurrentHealth (100.0f),
	MaxHealth (100.0f),
	ArmorDurability (100.0f),
	MaxArmorDurability (100.0f)

{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UFPShooterHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// 현재 생명력을 최대 생명력과 동일하게 설정.
	CurrentHealth = MaxHealth;

	// 현재 방어구 내구도를 최대 방어구 내구도와 동일하게 설정.
	ArmorDurability = MaxArmorDurability;
}

// 방어구 내구도에 따른 피해 가중치를 반환.
float UFPShooterHealthComponent::CheckArmorState()
{
	float DamageBias;

	if (ArmorDurability > 0.0f)
	{
		if (AsPercentage(ArmorDurability, MaxArmorDurability) <= 50.0f)
		{
			if (AsPercentage(ArmorDurability, MaxArmorDurability) <= 25.0f)
			{
				DamageBias = 0.75f;
			}
			else
			{
				DamageBias = 0.5f;
			}			
		}
		else
		{
			DamageBias = 0.0f;
		}
	}
	else
	{
		DamageBias = 1.0f;
	}

	return DamageBias;
}

// 받은 피해를 이용해서 방어구의 내구도와 캐릭터의 생명력을 처리.
void UFPShooterHealthComponent::TakeDamage(float DamageAmount)
{
	float DamageMultiplier;

	DamageMultiplier = DamageAmount * CheckArmorState();

	ApplyHealthDamage(DamageMultiplier);

	ApplyArmorDamage(DamageAmount - DamageMultiplier);	

	/* DEBUG_LINE */
	UE_LOG(LogTemp, Warning, TEXT("Health is: %f"), CurrentHealth);
	UE_LOG(LogTemp, Warning, TEXT("Armor is: %f"), ArmorDurability);
}

// 받은 피해를 이용해서 생명력을 처리.
void UFPShooterHealthComponent::ApplyHealthDamage(float DamageAmount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);
}

// 생명력을 회복.
void UFPShooterHealthComponent::Heal(float HealAmount)
{
	if (CurrentHealth > 0.0f)
	{		
		CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.0f, MaxHealth);
	}
}

// 받은 피해를 이용해서 방어구의 내구도를 처리.
void UFPShooterHealthComponent::ApplyArmorDamage(float DamageAmount)
{
	ArmorDurability = FMath::Clamp(ArmorDurability - DamageAmount, 0.0, MaxArmorDurability);
}

float UFPShooterHealthComponent::AsPercentage(float A, float B)
{
	return (A / B) * 100;
}

// 방어구 내구도를 회복.
void UFPShooterHealthComponent::RepairArmor(float ArmorAmount)
{
	if (ArmorDurability > 0.0f)
	{
		ArmorDurability = FMath::Clamp(ArmorDurability + ArmorAmount, 0.0f, MaxArmorDurability);
	}	
}

// Called every frame
void UFPShooterHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

