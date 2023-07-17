// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPShooterItem.h"
#include "WeaponProperties.h"
#include "FPShooterWeapon.generated.h"

// 무기의 등급
UENUM(BlueprintType)
enum class EWeaponGrade : uint8
{
    EWG_Normal UMETA(DisplayName = "Normal"),
    EWG_Improved UMETA(DisplayName = "Improved"),
    EWG_Enhanced UMETA(DisplayName = "Enhanced"),

    EWG_MAX UMETA(DisplayName = "Default")
};

// 무기의 발사 타입
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
    // 생성자
    AFPShooterWeapon();

private:
    // 무기 타입
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    EWeaponType WeaponType;

    // 무기 등급
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    EWeaponGrade WeaponGrade;

    // 총알 타입
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    EAmmoType AmmoType;

    // 무기 보관소 번호
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    int32 WeaponSlotIndex;

    // 무기의 사거리.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    float Distance;

    // 무기가 동시에 생성할 수 있는 라인 추적의 수
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    int32 RayNums;

    // 무기의 Muzzle Socket.
    const class USkeletalMeshSocket* MuzzleSocket;

    // 무기의 Muzzle Socket 이름.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    FName MuzzleSocketName;

    // 무기의 발사 타입
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    EWeaponFireType FireType;

    // 연속 발사 주기
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    float FireRate;

    // 무기의 ADS모드 실행 여부.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    bool bIsPossibleADS;

    // 무기의 탄창 크기
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo", meta = (AllowPrivateAccess = "true"))
    int32 MagazineCapacity;

    // 발사체 클래스.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class AFPShooterProjectile> ProjectileClass;

    // 발사체를 발사하는 힘.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    float LaunchForce;

    // 사용 가능한 총알 개수
    int32 CurrentAmmoAmount;

    // 총기 반동 수치.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    float WeaponRecoil;

    // 총기 반동 범위.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    FVector2D WeaponRecoilSpread;

    // Muzzle Flash
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Effect", meta = (AllowPrivateAccess = "true"))
    class UNiagaraSystem* MuzzleFlash;

    // 격발 효과음
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Effect", meta = (AllowPrivateAccess = "true"))
    class USoundBase* FireSound;  

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
   
public:
    // 무기 타입에 대한 외부 접근 함수.
    FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }

    // 무기가 사용하는 총알의 타입에 대한 외부 접근 함수
    FORCEINLINE EAmmoType GetAmmoType() const { return AmmoType; }

    // 탄창 크기에 대한 외부 접근 함수.
    FORCEINLINE int32 GetMagazineCapacity() const { return MagazineCapacity; }

    // 현재 총알 개수에 대한 외부 접근 함수.
    FORCEINLINE int32 GetCurrentAmmoAmount() const { return CurrentAmmoAmount; }

    // 총기 반동 수치에 대한 외부 접근 함수.
    FORCEINLINE float GetWeaponRecoil() const { return WeaponRecoil; }
    
    // 총기 반동 범위에 대한 외부 접근 함수.
    FORCEINLINE FVector2D GetWeaponRecoilSpread() const { return WeaponRecoilSpread; }

    // 무기 등급에 대한 외부 접근 함수.
    FORCEINLINE EWeaponGrade GetWeaponGrade() const { return WeaponGrade; }

    // 무기 슬롯 인덱스에 대한 외부 접근 함수.
    FORCEINLINE int32 GetWeaponSlotIndex() const { return WeaponSlotIndex; }

    // 무기의 Muzzle Socket에 대한 외부 접근 함수.
    FORCEINLINE const USkeletalMeshSocket* GetMuzzleSocket() const { return MuzzleSocket; }

    // 무기의 사거리에 대한 외부 접근 함수.
    FORCEINLINE float GetWeaponDistance() const { return Distance; }

    // Muzzle Flash에 대한 외부 접근 함수.
    FORCEINLINE UNiagaraSystem* GetMuzzleFlash() const { return MuzzleFlash; }

    // 결발 효과음에 대한 외부 접근 함수.
    FORCEINLINE USoundBase* GetFireSound() const { return FireSound; }

    // 발사 타입에 대한 외부 접근 함수.
    FORCEINLINE EWeaponFireType GetWeaponFireType() const { return FireType; }

    // 연속 발사 주기에 대한 외부 접근 함수.
    FORCEINLINE float GetFireRate() const { return FireRate; }

    // 동시에 생성 가능한 라인 추적의 수에 대한 외부 접근 함수.
    FORCEINLINE int32 GetRayNums() const { return RayNums; }

    // 무기의 ADS 모두 실행 여부에 대한 외부 접근 함수.
    FORCEINLINE bool GetPossibleADS() const { return bIsPossibleADS; }
    
    // 발사체를 발사하는 힘에 대한 외부 접근 함수.
    FORCEINLINE float GetLaunchForce() const { return LaunchForce; }

    // 무기의 총알 소비.
    void ConsumeAmmo(int32 Amount);

    // 탄창의 빈 공간 계산.
    int32 GetEmptyMagazineCapacity();

    // 탄창에 총알을 채운다.
    void ChargeAmmo(int32 Amount);

    // 발사체 생성.
    AFPShooterProjectile* SpawnProjectile(FVector Location, FRotator Rotation);

    // Called every frame
    virtual void Tick(float DeltaTime) override;
 };
