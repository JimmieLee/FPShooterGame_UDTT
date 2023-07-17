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

	// 루트 컴포넌트 생성.
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	if (RootScene)
	{		
		SetRootComponent(RootScene);
	}

	// 아이템 메시 생성.
	FPShooterItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Item Body"));
	if (FPShooterItemMesh)
	{
		FPShooterItemMesh->SetupAttachment(RootScene);
	}

	// 아이템 충돌체 생성.
	FPShooterItemCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Item Collision"));
	if (FPShooterItemCollision)
	{		
		FPShooterItemCollision->SetupAttachment(RootScene);
	}

	// 상호작용 충돌체 생성.
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
	// 감지된 액터가 영웅 클래스라면, AFPShooterHero로 Cast 한다.
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

	// 상호작용 가능한 충돌체에 대한 동적 감지 처리.
	FPShooterOverlapCollision->OnComponentBeginOverlap.AddDynamic(this, &AFPShooterItem::OnOverlapCollisionBeginOverlap);
	FPShooterOverlapCollision->OnComponentEndOverlap.AddDynamic(this, &AFPShooterItem::OnOverlapCollisionEndOverlap);
}

// Called every frame
void AFPShooterItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

