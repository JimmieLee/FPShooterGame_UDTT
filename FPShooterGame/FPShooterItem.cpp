// Fill out your copyright notice in the Description page of Project Settings.


#include "FPShooterItem.h"
#include "FPShooterHero.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"


// Sets default values
AFPShooterItem::AFPShooterItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ��Ʈ ������Ʈ ����.
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	if (RootScene)
	{		
		SetRootComponent(RootScene);
	}

	// ������ �޽� ����.
	FPShooterItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Item Body"));
	if (FPShooterItemMesh)
	{
		FPShooterItemMesh->SetupAttachment(RootScene);
	}

	// ������ �浹ü ����.
	FPShooterItemCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Item Collision"));
	if (FPShooterItemCollision)
	{		
		FPShooterItemCollision->SetupAttachment(RootScene);
	}

	// ��ȣ�ۿ� �浹ü ����.
	FPShooterOverlapCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Interfaction Collision"));
	if (FPShooterOverlapCollision)
	{
		FPShooterOverlapCollision->SetupAttachment(RootScene);
		FPShooterOverlapCollision->SetCollisionProfileName(FName("OverlapAllDynamic"));
		FPShooterOverlapCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void AFPShooterItem::OnOverlapCollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// ������ ���Ͱ� ���� Ŭ�������, AFPShooterHero�� Cast �Ѵ�.
	if (OtherActor && OtherActor->IsA(AFPShooterHero::StaticClass()))
	{
		AFPShooterHero* ShooterHero = Cast<AFPShooterHero>(OtherActor);

		if (ShooterHero)
		{
			ShooterHero->PickUpItem(this);
		}
	}		
}

void AFPShooterItem::OnOverlapCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlapp End"));
}

// Called when the game starts or when spawned
void AFPShooterItem::BeginPlay()
{
	Super::BeginPlay();

	// ��ȣ�ۿ� ������ �浹ü�� ���� ���� ���� ó��.
	FPShooterOverlapCollision->OnComponentBeginOverlap.AddDynamic(this, &AFPShooterItem::OnOverlapCollisionBeginOverlap);
	FPShooterOverlapCollision->OnComponentEndOverlap.AddDynamic(this, &AFPShooterItem::OnOverlapCollisionEndOverlap);
}

// Called every frame
void AFPShooterItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

