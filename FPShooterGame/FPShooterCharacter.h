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
	// 생명 관리자 컴포넌트.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UFPShooterHealthComponent* HealthComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// 생명 괸리 컴포넌트에 대한 외부 접근 함수
	FORCEINLINE UFPShooterHealthComponent* GetHealthComponent() const { return HealthComponent; }
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
