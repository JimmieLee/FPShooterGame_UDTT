// Fill out your copyright notice in the Description page of Project Settings.


#include "FPShooterEnemy.h"
#include "BulletImpactComponent.h"
#include "FPShooterHealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"

AFPShooterEnemy::AFPShooterEnemy()
{
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(
			ECollisionChannel::ECC_Visibility,
			ECollisionResponse::ECR_Ignore);
	}
	
	if (GetMesh())
	{
		GetMesh()->SetCollisionResponseToChannel(
			ECollisionChannel::ECC_Visibility,
			ECollisionResponse::ECR_Block);

		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}	

	// �ǰ� ��, ������ Physics Material�� ���� �ٸ� ȿ���� �����ϴ� ������Ʈ ����.
	BulletImpact = CreateDefaultSubobject<UBulletImpactComponent>(TEXT("Impact Effect Component"));
	
	// Health ���� ������Ʈ�� ����.
	HealthWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Point"));
	if (HealthWidgetComp)
	{
		// Health ���� ������Ʈ�� ���� ������ ȭ�� �������� ����.
		HealthWidgetComp->SetupAttachment(GetMesh());
		HealthWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);		

		if (HealthWidgetClass)
		{
			// Health ���� Ŭ������ ��ȿ�ϸ�, Health ���� ������Ʈ�� Ŭ������ ���.
			HealthWidget = Cast<UUserWidget>(HealthWidgetClass);
			HealthWidgetComp->SetWidgetClass(HealthWidgetClass);
		}
	}	
}

void AFPShooterEnemy::BeginPlay()
{
	Super::BeginPlay();	

	// ���� Health Point�� %�� ���Ѵ�.
	HealthPointPercentage = GetHealthComponent()->AsPercentage(
		GetHealthComponent()->GetCurrentHealth(),
		GetHealthComponent()->GetMaxHealth()) * 0.01f;	
}

void AFPShooterEnemy::Death()
{
	GetMesh()->SetSimulatePhysics(true);
}

void AFPShooterEnemy::PlayHitReactMontage()
{
	// �� ĳ���� �޽��� �ִ� �ν��Ͻ��� ���Ѵ�.
	UAnimInstance* HitReactAnimInstance = GetMesh()->GetAnimInstance();

	if (HitReactAnimInstance && HitReactMontage)
	{
		// �ִ� �ν��Ͻ��� �ǰ� ��Ÿ�ְ� ��ȿ�ϴٸ�, ������ ������ ��Ÿ�ָ� ����.
		HitReactAnimInstance->Montage_Play(HitReactMontage);
		HitReactAnimInstance->Montage_JumpToSection(FName(TEXT("HitFront")));

		/* DEBUG_LINE */
		UE_LOG(LogTemp, Warning, TEXT("Hit Montage Play"));
	}
}

void AFPShooterEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AFPShooterEnemy::TraceHit_Implementation(FHitResult TraceHit)
{
	if (BulletImpact)
	{
		BulletImpact->PlayImpactEffects(TraceHit);
	}	

	if (GetHealthComponent()->GetCurrentHealth() <= 0.0f)
	{
		Death();
	}

	PlayHitReactMontage();

	// ���� Health Point�� %�� ���Ѵ�.
	HealthPointPercentage = GetHealthComponent()->AsPercentage(
		GetHealthComponent()->GetCurrentHealth(),
		GetHealthComponent()->GetMaxHealth()) * 0.01f;
}
