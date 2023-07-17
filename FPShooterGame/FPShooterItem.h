// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPShooterItem.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_Weapon UMETA(DisplayName = "Weapon"),
	EIT_Ammo UMETA(DisplayName = "Ammo Box"),
	EIT_Health UMETA(DisplayName = "Health Pack"),
	EIT_Shield UMETA(DisplayName = "Shield Pack"),
	EIT_Stamina UMETA(DisplayName = "Stamina Pack"),
	EIT_Energy UMETA(DisplayName = "Weapon Energy"),
	EIT_Coin UMETA(DisplayName = "Coin"),
	EIT_Material UMETA(DisplayName = "Material"),
	EIT_Memorial UMETA(DisplayName = "Memorial Piece"),

	EIT_MAX UMETA(DisplayName = "Default Item")
};

UCLASS()
class FPSHOOTERGAME_API AFPShooterItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPShooterItem();

private:
	// 루트 컴포넌트.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootScene;

	// 아이템 메시
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* FPShooterItemMesh;

	// 아이템 충돌체.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* FPShooterItemCollision;

	// 상호 작용 충돌체.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* FPShooterOverlapCollision;

	// 아이템 타입.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemType ItemType;

	// 아이템 개수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	int32 ItemAmount;

protected:
	// 액터가 감지되었을 때 처리.
	UFUNCTION()
	void OnOverlapCollisionBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	// 액터가 감지되지 않을 때 처리.
	UFUNCTION()
	void OnOverlapCollisionEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// 아이템 메시에 대한 외부 접근
	FORCEINLINE USkeletalMeshComponent* GetItemMesh() const { return FPShooterItemMesh; }
	
	// 아이템 충돌체 컴포넌트에 대한 외부 접근.
	FORCEINLINE UCapsuleComponent* GetItemCollision() const { return FPShooterItemCollision; }

	// 상호 작용 충돌체 컴포넌트에 대한 외부 접근.
	FORCEINLINE USphereComponent* GetOverlapCollision() const { return FPShooterOverlapCollision; }
	
	// 아이템 타입에 대한 외부 접근.
	FORCEINLINE EItemType GetItemType() const { return ItemType; }

	// 아이템 개수에 대한 외부 접근.
	FORCEINLINE int32 GetItemAmount() const { return ItemAmount; }

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
