// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPShooterItem.h"
#include "WeaponProperties.h"
#include "FPShooterWeapon.generated.h"

// ������ ���
UENUM(BlueprintType)
enum class EWeaponGrade : uint8
{
    EWG_Normal UMETA(DisplayName = "Normal"),
    EWG_Improved UMETA(DisplayName = "Improved"),
    EWG_Enhanced UMETA(DisplayName = "Enhanced"),

    EWG_MAX UMETA(DisplayName = "Default")
};

// ������ �߻� Ÿ��
UENUM(BlueprintType)
enum class EWeaponFireType : uint8
{
    EWFT_Auto UMETA(DisplayName = "Automatic"),
    EWFT_Manual UMETA(DisplayName = "Manual"),
    EWFT_Spread UMETA(DisplayName = "Spread"),
    EWFT_Projectile UMETA(DisplayName = "Projectile"),

    EWFT_MAX UMETA(DisplayName = "Default")
};

UCLASS()
class FPSHOOTERGAME_API AFPShooterWeapon : public AFPShooterItem
{
	GENERATED_BODY()
	
public:
    // ������
    AFPShooterWeapon();

private:
    // ���� Ÿ��
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    EWeaponType WeaponType;

    // ���� ���
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    EWeaponGrade WeaponGrade;

    // �Ѿ� Ÿ��
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    EAmmoType AmmoType;

    // ���� ������ ��ȣ
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    int32 WeaponSlotIndex;

    // ������ ��Ÿ�.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    float Distance;

    // ���Ⱑ ���ÿ� ������ �� �ִ� ���� ������ ��
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    int32 RayNums;

    // ������ Muzzle Socket.
    const class USkeletalMeshSocket* MuzzleSocket;

    // ������ Muzzle Socket �̸�.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    FName MuzzleSocketName;

    // ������ �߻� Ÿ��
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    EWeaponFireType FireType;

    // ���� �߻� �ֱ�
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    float FireRate;

    // ������ ADS��� ���� ����.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    bool bIsPossibleADS;

    // ������ źâ ũ��
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo", meta = (AllowPrivateAccess = "true"))
    int32 MagazineCapacity;

    // �߻�ü Ŭ����.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class AFPShooterProjectile> ProjectileClass;

    // �߻�ü�� �߻��ϴ� ��.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    float LaunchForce;

    // ��� ������ �Ѿ� ����
    int32 CurrentAmmoAmount;

    // �ѱ� �ݵ� ��ġ.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    float WeaponRecoil;

    // �ѱ� �ݵ� ����.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    FVector2D WeaponRecoilSpread;

    // Muzzle Flash
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Effect", meta = (AllowPrivateAccess = "true"))
    class UNiagaraSystem* MuzzleFlash;

    // �ݹ� ȿ����
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Effect", meta = (AllowPrivateAccess = "true"))
    class USoundBase* FireSound;  

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
   
public:
    // ���� Ÿ�Կ� ���� �ܺ� ���� �Լ�.
    FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }

    // ���Ⱑ ����ϴ� �Ѿ��� Ÿ�Կ� ���� �ܺ� ���� �Լ�
    FORCEINLINE EAmmoType GetAmmoType() const { return AmmoType; }

    // źâ ũ�⿡ ���� �ܺ� ���� �Լ�.
    FORCEINLINE int32 GetMagazineCapacity() const { return MagazineCapacity; }

    // ���� �Ѿ� ������ ���� �ܺ� ���� �Լ�.
    FORCEINLINE int32 GetCurrentAmmoAmount() const { return CurrentAmmoAmount; }

    // �ѱ� �ݵ� ��ġ�� ���� �ܺ� ���� �Լ�.
    FORCEINLINE float GetWeaponRecoil() const { return WeaponRecoil; }
    
    // �ѱ� �ݵ� ������ ���� �ܺ� ���� �Լ�.
    FORCEINLINE FVector2D GetWeaponRecoilSpread() const { return WeaponRecoilSpread; }

    // ���� ��޿� ���� �ܺ� ���� �Լ�.
    FORCEINLINE EWeaponGrade GetWeaponGrade() const { return WeaponGrade; }

    // ���� ���� �ε����� ���� �ܺ� ���� �Լ�.
    FORCEINLINE int32 GetWeaponSlotIndex() const { return WeaponSlotIndex; }

    // ������ Muzzle Socket�� ���� �ܺ� ���� �Լ�.
    FORCEINLINE const USkeletalMeshSocket* GetMuzzleSocket() const { return MuzzleSocket; }

    // ������ ��Ÿ��� ���� �ܺ� ���� �Լ�.
    FORCEINLINE float GetWeaponDistance() const { return Distance; }

    // Muzzle Flash�� ���� �ܺ� ���� �Լ�.
    FORCEINLINE UNiagaraSystem* GetMuzzleFlash() const { return MuzzleFlash; }

    // ��� ȿ������ ���� �ܺ� ���� �Լ�.
    FORCEINLINE USoundBase* GetFireSound() const { return FireSound; }

    // �߻� Ÿ�Կ� ���� �ܺ� ���� �Լ�.
    FORCEINLINE EWeaponFireType GetWeaponFireType() const { return FireType; }

    // ���� �߻� �ֱ⿡ ���� �ܺ� ���� �Լ�.
    FORCEINLINE float GetFireRate() const { return FireRate; }

    // ���ÿ� ���� ������ ���� ������ ���� ���� �ܺ� ���� �Լ�.
    FORCEINLINE int32 GetRayNums() const { return RayNums; }

    // ������ ADS ��� ���� ���ο� ���� �ܺ� ���� �Լ�.
    FORCEINLINE bool GetPossibleADS() const { return bIsPossibleADS; }
    
    // �߻�ü�� �߻��ϴ� ���� ���� �ܺ� ���� �Լ�.
    FORCEINLINE float GetLaunchForce() const { return LaunchForce; }

    // ������ �Ѿ� �Һ�.
    void ConsumeAmmo(int32 Amount);

    // źâ�� �� ���� ���.
    int32 GetEmptyMagazineCapacity();

    // źâ�� �Ѿ��� ä���.
    void ChargeAmmo(int32 Amount);

    // �߻�ü ����.
    AFPShooterProjectile* SpawnProjectile(FVector Location, FRotator Rotation);

    // Called every frame
    virtual void Tick(float DeltaTime) override;
 };
