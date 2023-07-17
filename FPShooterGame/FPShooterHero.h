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
	/* 컴포넌트 카테고리------------------------------------------------------------------------------------ */
	// FPS 카메라 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FPShooterCamera;

	// FPS 캐릭터 메시
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* FPShooterMesh;


	/* 움직임 카테고리------------------------------------------------------------------------------------- */
	// 기본 회전
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate;

	// 스태미너 (지구력)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Stamina;

	// 질주 실행 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsSprint;	

	// 점프 실행 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsJump;

	// 영웅 이동 속도 (기본 수치 보존)
	float HeroWalkSpeed;	


	/* 전투 카테고리-------------------------------------------------------------------------------------- */
	// 트리거 버튼 입력 여부.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool bIsPullTheTrigger;

	// 크로스헤어 UI 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> HUDCrosshairClass;

	// 메인 HUD 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> MainHUDClass;

	FHitResult HitTracedTarget;

	// 크로스헤어 UI
	class UUserWidget* HUDCrosshair;

	// 메인 HUD UI
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (BindWidget, AllowPrivateAccess = "true"))
	class UUserWidget* MainHUD;
	
	// 발사 애님 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* FireMontage;

	// ADS 실행 시, 발사 애님 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* AimFireMontage;

	// Relaod 애님 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* ReloadMontage;	

	// ADS 실행 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool bIsADS;	

	// Reload 실행 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool bIsReload;

	// Reload 실행 타이머.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	FTimerHandle ReloadTimer;

	// Reload 실행 시간.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float ReloadTime;

	// ADS 카메라 FOV
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float ADSCameraView;	

	// 기본 카메라 높이
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float BaseCameraHeight;

	// Crouch 카메라 높이
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float CrouchCameraHeight;

	// 카메라 뷰 보간.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float FPShooterCameraView;

	// 기본 메시 높이
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float BaseMeshHeight;	


	/* 무기 카테고리-------------------------------------------------------------------------------------- */
	// 장착된 무기.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	class AFPShooterWeapon* EquippedWeapon;

	// 기본 무기 지정 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AFPShooterWeapon> DefaultWeaponClass;

	// 무기 저장소
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TArray<class AFPShooterWeapon*> WeaponSlot;	

	// 현재 무기의 슬롯 인덱스 (무기 교체 시, 인덱스 저장)
	int32 CurrentWeaponSlotIndex;

	// 연속 발사 타이머
	FTimerHandle WeaponFireTimer;

	// 무기 발사 주기 종료 체크.
	bool bIsFiring;


	/* 총알 카테고리----------------------------------------------------------------------------------------*/
	// 총알의 타입 별로 보관하고 있는 총알의 개수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ammo", meta = (AllowPrivateAccess = "true"))
	TMap<EAmmoType, int32> AmmoPack;

	// 피스톨 총알의 최대 보관 개수.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo", meta = (AllowPrivateAccess = "true"))
	int32 PistolAmmoMax;

	// Assult Rifle 총알의 최대 보관 개수.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo", meta = (AllowPrivateAccess = "true"))
	int32 ARAmmoMax;

	// Shotgun 총알의 최대 보관 개수.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo", meta = (AllowPrivateAccess = "true"))
	int32 ShotgunAmmoMax;

	// Grenade Launcher 총알의 최대 보관 개수.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo", meta = (AllowPrivateAccess = "true"))
	int32 GLAmmoMax;

	// Rocket Launcher 총알의 최대 보관 개수.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo", meta = (AllowPrivateAccess = "true"))
	int32 RLAmmoMax;

	// Sniper Rifle 총알의 최대 보관 개수.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo", meta = (AllowPrivateAccess = "true"))
	int32 SRAmmoMax;


	/* 크로스 헤어 카테고리-------------------------------------------------------------------------------- */
	// 크로스헤어 확산 가중치
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
	float CrosshairSpreadMultiplier;

	// 점프 시, 크로스헤어 확산 가중치.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
	float CrosshairSpreadJumpFactor;

	// 조준 시, 크로스헤어 확산 가중치.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
	float CrosshairSpreadADSFactor;

	// 격발 시, 크로스헤어 확산 가중치.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
	float CrosshairSpreadShootingFactor;	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 스태미너 측량.
	void CalculateStamina();	

	// 크로스헤어 확산 가중치 측량.
	void CalcuateCrosshairSpreadMultiplier(float DeltaTime);

	// 카메라의 Default/ ADS 전환 보간.
	void ADSInterpolationCamera(float DeltaTime);

	// 크로스헤어 표시 처리.
	void HiddenCrosshair(bool bIsHidden);

	// Crouch 상태 전환 보간.
	void CrouchInterpolationStatus(float DeltaTime);

	// Reload 실행.
	void StartReload();

	// Rload 타이머 종료 Callback.
	void FinishReload();

	// 기본 무기 생성.
	AFPShooterWeapon* SpawnDefaultWeapon();

	// 무기 장착 
	void EquipWeapon(AFPShooterWeapon* Weapon);

	// 장착된 무기를 새로운 무기로 교체하여 장착
	void SwapWeapon(int32 WeaponIndex);

	// 동일한 타입의 무기 보유 여부를 검사
	bool HasAleadyPickUpWeapon(AFPShooterWeapon* PickUpWeapon);

	// 무기를 픽업.
	void PickUpWeapon(AFPShooterWeapon* Weapon);

	// 크로스헤어의 화면 위치를 구한다.
	bool GetCrosshairWorldPoint(FVector& CrosshairWorldLocation, FVector& CrosshairWorldDirection);
		
	// 발사 추적 위치 찾기
	bool GetFireTracePoint(const FVector& MuzzleSocketLocation, FVector& TraceLineLocation);

	// 획득한 총알을 보관.
	void StoreAmmo(EAmmoType AmmoType, int32 Amount);

	// 무기 인텍스 (테스트)
	int32 index = 0;

	// 발사 타이머 실행.
	void StartWeaponFire();

	// 일반 발사 타이머 종료 Callback
	void FinishFire();

	// 자동 발사 타이머 종료 Callback
	void FinishAutoFire();

	// 발사 몽타주 재생.
	void PlayFireMontage();

	// 대상을 향해 발사
	void WeaponFire();

	// 대상을 향해 발사 (확산 타입)
	void WeaponSpreadFire();

	// 대상을 향해 발사 (발사체 타입)
	void WeaponProjectileFire();

	// 이전 무기로 교체.
	void SwitchPrevWeapon();

	// 다음 무기로 교체.
	void SwitchNextWeapon();

	// 저장되어 있는 무기 체크.
	void StoredWeapon();

	// 탄약을 픽업.
	void PickUpAmmo(class AFPShooterAmmo* Ammo);

public:
	// 영웅의 메시를 구할 수 있는 외부 접근 함수.
	FORCEINLINE USkeletalMeshComponent* GetShooterMesh() const { return FPShooterMesh;  }
	
	// 영웅의 질주 여부를 구할 수 있는 외부 접근 함수.
	FORCEINLINE bool GetSprintStatus() const { return bIsSprint; }

	// 영웅의 ADS 여부를 구할 수 있는 외부 접근 함수.
	FORCEINLINE bool GetADSStatus() const { return bIsADS; }	

	// 영웅이 장착한 무기를 구할 수 있는 외부 접근 함수.
	FORCEINLINE AFPShooterWeapon* GetEquippedWeapon() const { return EquippedWeapon; }

	// 최종 계산된 크로스헤어 확산 가중치를 구한다. (위젯 블루프린트에서 호출)
	UFUNCTION(BlueprintPure)
	float GetCrosshairSpreadMultiplier();

	// 아이템을 픽업한다.
	void PickUpItem(class AFPShooterItem* Item);

	// 영웅을 앞/ 뒤로 이동.
	void HeroMoveForward(float AxisValue);

	// 영웅을 왼쪽/ 오른쪽으로 이동.
	void HeroMoveRight(float AxisValue);

	// 영웅의 시야를 위/ 아래로 회전. (마우스 입력)
	void HeroLookUp(float AxisValue);

	// 영웅의 시야를 위/ 아래로 회전. (게임패드 입력)
	void HeroLookUpGamepad(float AxisValue);

	// 영웅의 시야를 왼쪽/ 오른쪽으로 회전. (마우스 입력)
	void HeroTurn(float AxisValue);

	// 영웅의 시야를 왼쪽/ 오른쪽으로 회전. (게임패드 입력)
	void HeroTurnGamepad(float AxisValue);

	// 영웅의 점프 시작.
	void JumpPressed();

	// 영웅의 점프 종료.
	void JumpReleased();
	
	// 영웅의 질주 시작.
	void SpirntPressed();

	// 영웅의 질주 종료.
	void SprintReleased();
	
	// 트리거 버튼 입력.
	void TriggerPressed();

	// 트리거 버튼 입력 해제.
	void TriggerReleased();

	// ADS 실행 버튼 입력.
	void ADSPressed();

	// ADS 실행 버튼 입력 해재.
	void ADSReleased();

	// Crouch 실행 버튼 입력 토글 On/Off
	void CrouchPressed();

	// Reload 실행 버튼 입력.
	void ReloadPressed();

	// 이전 슬롯의 무기로 교체 입력.
	void SwitchPrevWeaponPressed();

	// 다음 슬롯의 무기로 교체 입력.
	void SwitchNextWeaponPressed();
	
	// 무기 선택 여부에 따른 썸네일의 색상을 변경하기 위한, 블루프린트 구현 함수.
	UFUNCTION(BlueprintImplementableEvent)
	void EquippedWeaponSlot(int32 SlotIndex);

	// 무기 픽업 여부에 따른 썸네일 활성을 위한, 블루프린트 구현 함수.
	UFUNCTION(BlueprintImplementableEvent)
	void PickedUpWeaponSlot(int32 SlotIndex);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
