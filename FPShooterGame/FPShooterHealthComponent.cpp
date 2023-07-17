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
	
	// ���� ������� �ִ� ����°� �����ϰ� ����.
	CurrentHealth = MaxHealth;

	// ���� �� �������� �ִ� �� �������� �����ϰ� ����.
	ArmorDurability = MaxArmorDurability;
}

// �� �������� ���� ���� ����ġ�� ��ȯ.
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

// ���� ���ظ� �̿��ؼ� ���� �������� ĳ������ ������� ó��.
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

// ���� ���ظ� �̿��ؼ� ������� ó��.
void UFPShooterHealthComponent::ApplyHealthDamage(float DamageAmount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);
}

// ������� ȸ��.
void UFPShooterHealthComponent::Heal(float HealAmount)
{
	if (CurrentHealth > 0.0f)
	{		
		CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.0f, MaxHealth);
	}
}

// ���� ���ظ� �̿��ؼ� ���� �������� ó��.
void UFPShooterHealthComponent::ApplyArmorDamage(float DamageAmount)
{
	ArmorDurability = FMath::Clamp(ArmorDurability - DamageAmount, 0.0, MaxArmorDurability);
}

float UFPShooterHealthComponent::AsPercentage(float A, float B)
{
	return (A / B) * 100;
}

// �� �������� ȸ��.
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

