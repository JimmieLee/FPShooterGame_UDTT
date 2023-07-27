#include "FPShooterAIController.h"
// Fill out your copyright notice in the Description page of Project Settings.


#include "FPShooterAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"


AFPShooterAIController::AFPShooterAIController()
{
	// AI Perception ������Ʈ ����.
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));

	AISenseSight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AI Sense Sight"));
	if (AISenseSight)
	{
		// �ð� ������ ���� ����.
		AISenseSight->SightRadius = 1000.0f;

		// ���� ��ü���Ը� �ð� ���� ���͸�.
		AISenseSight->DetectionByAffiliation.bDetectEnemies = true;
		AISenseSight->DetectionByAffiliation.bDetectFriendlies = false;
		AISenseSight->DetectionByAffiliation.bDetectNeutrals = false;
	}

	AISenseHearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("AI Sense Hearing"));
	if (AISenseHearing)
	{
		// û�� ������ ���� ����.
		AISenseHearing->HearingRange = 2000.0f;

		// ���� ��ü���Ը� û�� ���� ���͸�
		AISenseHearing->DetectionByAffiliation.bDetectEnemies = true;
		AISenseHearing->DetectionByAffiliation.bDetectFriendlies = false;
		AISenseHearing->DetectionByAffiliation.bDetectNeutrals = false;
	}

	if (AIPerceptionComponent)
	{
		// AI Perception�� �ð� ������ û�� ������ ����.
		AIPerceptionComponent->ConfigureSense(*AISenseSight);
		AIPerceptionComponent->ConfigureSense(*AISenseHearing);

		// ������ ���� ��, �ֵ����� ������ ����.
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
