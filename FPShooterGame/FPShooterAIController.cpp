#include "FPShooterAIController.h"
// Fill out your copyright notice in the Description page of Project Settings.


#include "FPShooterAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"


AFPShooterAIController::AFPShooterAIController()
{
	// AI Perception 컴포넌트 생성.
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));

	AISenseSight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AI Sense Sight"));
	if (AISenseSight)
	{
		// 시각 센스의 범위 설정.
		AISenseSight->SightRadius = 1000.0f;

		// 적대 개체에게만 시각 센스 필터링.
		AISenseSight->DetectionByAffiliation.bDetectEnemies = true;
		AISenseSight->DetectionByAffiliation.bDetectFriendlies = false;
		AISenseSight->DetectionByAffiliation.bDetectNeutrals = false;
	}

	AISenseHearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("AI Sense Hearing"));
	if (AISenseHearing)
	{
		// 청각 센스의 범위 설정.
		AISenseHearing->HearingRange = 2000.0f;

		// 적대 개체에게만 청각 센스 필터링
		AISenseHearing->DetectionByAffiliation.bDetectEnemies = true;
		AISenseHearing->DetectionByAffiliation.bDetectFriendlies = false;
		AISenseHearing->DetectionByAffiliation.bDetectNeutrals = false;
	}

	if (AIPerceptionComponent)
	{
		// AI Perception에 시각 센스와 청각 센스를 적용.
		AIPerceptionComponent->ConfigureSense(*AISenseSight);
		AIPerceptionComponent->ConfigureSense(*AISenseHearing);

		// 적용한 센스 중, 주도적인 센스를 설정.
		AIPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	}
}

void AFPShooterAIController::BeginPlay()
{
	Super::BeginPlay();

}

void AFPShooterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}
