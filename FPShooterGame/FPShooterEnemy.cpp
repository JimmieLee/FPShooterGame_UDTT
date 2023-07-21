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

	// 피격 시, 설정된 Physics Material에 따라 다른 효과를 생성하는 컴포넌트 생성.
	BulletImpact = CreateDefaultSubobject<UBulletImpactComponent>(TEXT("Impact Effect Component"));
	
	// Health 위젯 컴포넌트를 생성.
	HealthWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Point"));
	if (HealthWidgetComp)
	{
		// Health 위젯 컴포넌트의 위젯 공간을 화면 공간으로 설정.
		HealthWidgetComp->SetupAttachment(GetMesh());
		HealthWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);		

		if (HealthWidgetClass)
		{
			// Health 위젯 클래스가 유효하면, Health 위젯 컴포넌트에 클래스를 등록.
			HealthWidget = Cast<UUserWidget>(HealthWidgetClass);
			HealthWidgetComp->SetWidgetClass(HealthWidgetClass);
		}
	}	
}

void AFPShooterEnemy::BeginPlay()
{
	Super::BeginPlay();	

	// 최종 Health Point의 %를 구한다.
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
	// 적 캐릭터 메시의 애님 인스턴스를 구한다.
	UAnimInstance* HitReactAnimInstance = GetMesh()->GetAnimInstance();

	if (HitReactAnimInstance && HitReactMontage)
	{
		// 애님 인스턴스와 피격 몽타주가 유효하다면, 지정된 섹션의 몽타주를 실행.
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

	// 최종 Health Point의 %를 구한다.
	HealthPointPercentage = GetHealthComponent()->AsPercentage(
		GetHealthComponent()->GetCurrentHealth(),
		GetHealthComponent()->GetMaxHealth()) * 0.01f;
}
