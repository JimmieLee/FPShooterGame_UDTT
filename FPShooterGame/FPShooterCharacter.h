// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPShooterCharacter.generated.h"

UCLASS()
class FPSHOOTERGAME_API AFPShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPShooterCharacter();

private:
	// ���� ������ ������Ʈ.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UFPShooterHealthComponent* HealthComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// ���� ���� ������Ʈ�� ���� �ܺ� ���� �Լ�
	FORCEINLINE UFPShooterHealthComponent* GetHealthComponent() const { return HealthComponent; }
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
