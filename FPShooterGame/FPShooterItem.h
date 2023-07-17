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
	// ��Ʈ ������Ʈ.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootScene;

	// ������ �޽�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* FPShooterItemMesh;

	// ������ �浹ü.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* FPShooterItemCollision;

	// ��ȣ �ۿ� �浹ü.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* FPShooterOverlapCollision;

	// ������ Ÿ��.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemType ItemType;

	// ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	int32 ItemAmount;

protected:
	// ���Ͱ� �����Ǿ��� �� ó��.
	UFUNCTION()
	void OnOverlapCollisionBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	// ���Ͱ� �������� ���� �� ó��.
	UFUNCTION()
	void OnOverlapCollisionEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// ������ �޽ÿ� ���� �ܺ� ����
	FORCEINLINE USkeletalMeshComponent* GetItemMesh() const { return FPShooterItemMesh; }
	
	// ������ �浹ü ������Ʈ�� ���� �ܺ� ����.
	FORCEINLINE UCapsuleComponent* GetItemCollision() const { return FPShooterItemCollision; }

	// ��ȣ �ۿ� �浹ü ������Ʈ�� ���� �ܺ� ����.
	FORCEINLINE USphereComponent* GetOverlapCollision() const { return FPShooterOverlapCollision; }
	
	// ������ Ÿ�Կ� ���� �ܺ� ����.
	FORCEINLINE EItemType GetItemType() const { return ItemType; }

	// ������ ������ ���� �ܺ� ����.
	FORCEINLINE int32 GetItemAmount() const { return ItemAmount; }

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
