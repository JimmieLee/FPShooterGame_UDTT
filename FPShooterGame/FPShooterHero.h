// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPShooterCharacter.h"
#include "WeaponProperties.h"
#include "FPShooterHitInterface.h"
#include "FPShooterHero.generated.h"

UCLASS()
class FPSHOOTERGAME_API AFPShooterHero : public AFPShooterCharacter, public IFPShooterHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPShooterHero();

private:
	/* ������Ʈ ī�װ�------------------------------------------------------------------------------------ */
	// FPS ī�޶� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FPShooterCamera;

	// FPS ĳ���� �޽�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* FPShooterMesh;


	/* ������ ī�װ�------------------------------------------------------------------------------------- */
	// �⺻ ȸ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate;

	// ���¹̳� (������)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Stamina;

	// ���� ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsSprint;	

	// ���� ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsJump;

	// ���� �̵� �ӵ� (�⺻ ��ġ ����)
	float HeroWalkSpeed;	


	/* ���� ī�װ�-------------------------------------------------------------------------------------- */
	// Ʈ���� ��ư �Է� ����.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool bIsPullTheTrigger;

	// ũ�ν���� UI Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> HUDCrosshairClass;

	// ���� HUD Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> MainHUDClass;

	FHitResult HitTracedTarget;

	// ũ�ν���� UI
	class UUserWidget* HUDCrosshair;

	// ���� HUD UI
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (BindWidget, AllowPrivateAccess = "true"))
	class UUserWidget* MainHUD;
	
	// �߻� �ִ� ��Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* FireMontage;

	// ADS ���� ��, �߻� �ִ� ��Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* AimFireMontage;

	// Relaod �ִ� ��Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* ReloadMontage;	

	// ADS ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool bIsADS;	

	// Reload ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool bIsReload;

	// Reload ���� Ÿ�̸�.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	FTimerHandle ReloadTimer;

	// Reload ���� �ð�.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float ReloadTime;

	// ADS ī�޶� FOV
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float ADSCameraView;	

	// �⺻ ī�޶� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float BaseCameraHeight;

	// Crouch ī�޶� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float CrouchCameraHeight;

	// ī�޶� �� ����.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float FPShooterCameraView;

	// �⺻ �޽� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float BaseMeshHeight;	


	/* ���� ī�װ�-------------------------------------------------------------------------------------- */
	// ������ ����.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	class AFPShooterWeapon* EquippedWeapon;

	// �⺻ ���� ���� Ŭ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AFPShooterWeapon> DefaultWeaponClass;

	// ���� �����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TArray<class AFPShooterWeapon*> WeaponSlot;	

	// ���� ������ ���� �ε��� (���� ��ü ��, �ε��� ����)
	int32 CurrentWeaponSlotIndex;

	// ���� �߻� Ÿ�̸�
	FTimerHandle WeaponFireTimer;

	// ���� �߻� �ֱ� ���� üũ.
	bool bIsFiring;


	/* �Ѿ� ī�װ�----------------------------------------------------------------------------------------*/
	// �Ѿ��� Ÿ�� ���� �����ϰ� �ִ� �Ѿ��� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ammo", meta = (AllowPrivateAccess = "true"))
	TMap<EAmmoType, int32> AmmoPack;

	// �ǽ��� �Ѿ��� �ִ� ���� ����.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo", meta = (AllowPrivateAccess = "true"))
	int32 PistolAmmoMax;

	// Assult Rifle �Ѿ��� �ִ� ���� ����.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo", meta = (AllowPrivateAccess = "true"))
	int32 ARAmmoMax;

	// Shotgun �Ѿ��� �ִ� ���� ����.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo", meta = (AllowPrivateAccess = "true"))
	int32 ShotgunAmmoMax;

	// Grenade Launcher �Ѿ��� �ִ� ���� ����.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo", meta = (AllowPrivateAccess = "true"))
	int32 GLAmmoMax;

	// Rocket Launcher �Ѿ��� �ִ� ���� ����.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo", meta = (AllowPrivateAccess = "true"))
	int32 RLAmmoMax;

	// Sniper Rifle �Ѿ��� �ִ� ���� ����.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo", meta = (AllowPrivateAccess = "true"))
	int32 SRAmmoMax;


	/* ũ�ν� ��� ī�װ�-------------------------------------------------------------------------------- */
	// ũ�ν���� Ȯ�� ����ġ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
	float CrosshairSpreadMultiplier;

	// ���� ��, ũ�ν���� Ȯ�� ����ġ.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
	float CrosshairSpreadJumpFactor;

	// ���� ��, ũ�ν���� Ȯ�� ����ġ.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
	float CrosshairSpreadADSFactor;

	// �ݹ� ��, ũ�ν���� Ȯ�� ����ġ.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
	float CrosshairSpreadShootingFactor;	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// ���¹̳� ����.
	void CalculateStamina();	

	// ũ�ν���� Ȯ�� ����ġ ����.
	void CalcuateCrosshairSpreadMultiplier(float DeltaTime);

	// ī�޶��� Default/ ADS ��ȯ ����.
	void ADSInterpolationCamera(float DeltaTime);

	// ũ�ν���� ǥ�� ó��.
	void HiddenCrosshair(bool bIsHidden);

	// Crouch ���� ��ȯ ����.
	void CrouchInterpolationStatus(float DeltaTime);

	// Reload ����.
	void StartReload();

	// Rload Ÿ�̸� ���� Callback.
	void FinishReload();

	// �⺻ ���� ����.
	AFPShooterWeapon* SpawnDefaultWeapon();

	// ���� ���� 
	void EquipWeapon(AFPShooterWeapon* Weapon);

	// ������ ���⸦ ���ο� ����� ��ü�Ͽ� ����
	void SwapWeapon(int32 WeaponIndex);

	// ������ Ÿ���� ���� ���� ���θ� �˻�
	bool HasAleadyPickUpWeapon(AFPShooterWeapon* PickUpWeapon);

	// ���⸦ �Ⱦ�.
	void PickUpWeapon(AFPShooterWeapon* Weapon);

	// ũ�ν������ ȭ�� ��ġ�� ���Ѵ�.
	bool GetCrosshairWorldPoint(FVector& CrosshairWorldLocation, FVector& CrosshairWorldDirection);
		
	// �߻� ���� ��ġ ã��
	bool GetFireTracePoint(const FVector& MuzzleSocketLocation, FVector& TraceLineLocation);

	// ȹ���� �Ѿ��� ����.
	void StoreAmmo(EAmmoType AmmoType, int32 Amount);

	// ���� ���ؽ� (�׽�Ʈ)
	int32 index = 0;

	// �߻� Ÿ�̸� ����.
	void StartWeaponFire();

	// �Ϲ� �߻� Ÿ�̸� ���� Callback
	void FinishFire();

	// �ڵ� �߻� Ÿ�̸� ���� Callback
	void FinishAutoFire();

	// �߻� ��Ÿ�� ���.
	void PlayFireMontage();

	// ����� ���� �߻�
	void WeaponFire();

	// ����� ���� �߻� (Ȯ�� Ÿ��)
	void WeaponSpreadFire();

	// ����� ���� �߻� (�߻�ü Ÿ��)
	void WeaponProjectileFire();

	// ���� ����� ��ü.
	void SwitchPrevWeapon();

	// ���� ����� ��ü.
	void SwitchNextWeapon();

	// ����Ǿ� �ִ� ���� üũ.
	void StoredWeapon();

	// ź���� �Ⱦ�.
	void PickUpAmmo(class AFPShooterAmmo* Ammo);

public:
	// ������ �޽ø� ���� �� �ִ� �ܺ� ���� �Լ�.
	FORCEINLINE USkeletalMeshComponent* GetShooterMesh() const { return FPShooterMesh;  }
	
	// ������ ���� ���θ� ���� �� �ִ� �ܺ� ���� �Լ�.
	FORCEINLINE bool GetSprintStatus() const { return bIsSprint; }

	// ������ ADS ���θ� ���� �� �ִ� �ܺ� ���� �Լ�.
	FORCEINLINE bool GetADSStatus() const { return bIsADS; }	

	// ������ ������ ���⸦ ���� �� �ִ� �ܺ� ���� �Լ�.
	FORCEINLINE AFPShooterWeapon* GetEquippedWeapon() const { return EquippedWeapon; }

	// ���� ���� ũ�ν���� Ȯ�� ����ġ�� ���Ѵ�. (���� �������Ʈ���� ȣ��)
	UFUNCTION(BlueprintPure)
	float GetCrosshairSpreadMultiplier();

	// �������� �Ⱦ��Ѵ�.
	void PickUpItem(class AFPShooterItem* Item);

	// ������ ��/ �ڷ� �̵�.
	void HeroMoveForward(float AxisValue);

	// ������ ����/ ���������� �̵�.
	void HeroMoveRight(float AxisValue);

	// ������ �þ߸� ��/ �Ʒ��� ȸ��. (���콺 �Է�)
	void HeroLookUp(float AxisValue);

	// ������ �þ߸� ��/ �Ʒ��� ȸ��. (�����е� �Է�)
	void HeroLookUpGamepad(float AxisValue);

	// ������ �þ߸� ����/ ���������� ȸ��. (���콺 �Է�)
	void HeroTurn(float AxisValue);

	// ������ �þ߸� ����/ ���������� ȸ��. (�����е� �Է�)
	void HeroTurnGamepad(float AxisValue);

	// ������ ���� ����.
	void JumpPressed();

	// ������ ���� ����.
	void JumpReleased();
	
	// ������ ���� ����.
	void SpirntPressed();

	// ������ ���� ����.
	void SprintReleased();
	
	// Ʈ���� ��ư �Է�.
	void TriggerPressed();

	// Ʈ���� ��ư �Է� ����.
	void TriggerReleased();

	// ADS ���� ��ư �Է�.
	void ADSPressed();

	// ADS ���� ��ư �Է� ����.
	void ADSReleased();

	// Crouch ���� ��ư �Է� ��� On/Off
	void CrouchPressed();

	// Reload ���� ��ư �Է�.
	void ReloadPressed();

	// ���� ������ ����� ��ü �Է�.
	void SwitchPrevWeaponPressed();

	// ���� ������ ����� ��ü �Է�.
	void SwitchNextWeaponPressed();
	
	// ���� ���� ���ο� ���� ������� ������ �����ϱ� ����, �������Ʈ ���� �Լ�.
	UFUNCTION(BlueprintImplementableEvent)
	void EquippedWeaponSlot(int32 SlotIndex);

	// ���� �Ⱦ� ���ο� ���� ����� Ȱ���� ����, �������Ʈ ���� �Լ�.
	UFUNCTION(BlueprintImplementableEvent)
	void PickedUpWeaponSlot(int32 SlotIndex);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
