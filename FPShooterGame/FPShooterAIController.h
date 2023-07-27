// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FPShooterAIController.generated.h"


UCLASS()
class FPSHOOTERGAME_API AFPShooterAIController : public AAIController
{
	GENERATED_BODY()

public:

	AFPShooterAIController();

private:
	// AI Perception ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

	// AI �ð� Sense Config
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Sense", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAISenseConfig_Sight> AISenseSight;

	// AI û�� Sense Config
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Sense", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAISenseConfig_Hearing> AISenseHearing;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};