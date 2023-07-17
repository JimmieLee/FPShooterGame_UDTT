// Fill out your copyright notice in the Description page of Project Settings.


#include "FPShooterHero.h"
#include "FPShooterEnemy.h"
#include "FPShooterAnimInstance.h"
#include "FPShooterItem.h"
#include "FPShooterWeapon.h"
#include "FPShooterAmmo.h"
#include "FPShooterProjectile.h"
#include "WeaponProperties.h"
#include "FPShooterHealthComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "DrawDebugHelpers.h"

AFPShooterHero::AFPShooterHero() :
	BaseTurnRate(45.0f),
	Stamina(100.0),
	bIsSprint(false),
	bIsJump(false),
	HeroWalkSpeed(GetCharacterMovement()->MaxWalkSpeed),
	bIsPullTheTrigger(false),
	bIsADS(false),
	bIsReload(false),
	ReloadTime(0.0f),
	ADSCameraView(90.0f),
	BaseCameraHeight(60.0f),
	CrouchCameraHeight(34.0f),
	FPShooterCameraView(BaseCameraHeight),
	bIsFiring(false),
	// 총알 보관 제한
	PistolAmmoMax(48),
	ARAmmoMax(128),
	ShotgunAmmoMax(28),
	GLAmmoMax(24),
	RLAmmoMax(4),
	SRAmmoMax(28),
	// 크로스헤어 확산 가중치
	CrosshairSpreadMultiplier(0.0f),
	CrosshairSpreadJumpFactor(0.0f),
	CrosshairSpreadADSFactor(0.0f),
	CrosshairSpreadShootingFactor(0.0f)
{	
	// 플레이어 카메라 컴포넌트 생성.
	FPShooterCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	if (FPShooterCamera)
	{
		FPShooterCamera->SetupAttachment(RootComponent);
		FPShooterCamera->SetRelativeLocation(FVector(0.0f, 0.0f, BaseCameraHeight));
		FPShooterCamera->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	}

	// FPS 캐릭터 메시 컴포넌트 생성.
	FPShooterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShooterMesh"));
	if (FPShooterMesh)
	{
		FPShooterMesh->SetupAttachment(FPShooterCamera);
		FPShooterMesh->SetRelativeLocation(FVector(10.0f, 0.0f, BaseMeshHeight));
		FPShooterMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		FPShooterMesh->CastShadow = 0;
	}

	// 카메라가 컨트롤러의 회전을 사용하도록 설정.
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Crouch 가능
	GetCharacterMovement()->bWantsToCrouch = true;
 }

void AFPShooterHero::BeginPlay()
{
	Super::BeginPlay();		
	
	// 보유 탄약 초기화 
	AmmoPack.Add(EAmmoType::EAT_Pistol, 0);
	AmmoPack.Add(EAmmoType::EAT_AR, 0);
	AmmoPack.Add(EAmmoType::EAT_Shotgun, 0);
	AmmoPack.Add(EAmmoType::EAT_GL, 0);
	AmmoPack.Add(EAmmoType::EAT_RL, 0);
	AmmoPack.Add(EAmmoType::EAT_SR, 0);
	
	// 기본 무기(피스톨)의 예비 탄약 보유 수량 설정.
	StoreAmmo(EAmmoType::EAT_Pistol, 10);
	
	if (HUDCrosshairClass)
	{
		// 크로스헤어 위젯 생성 및 화면에 추가.
		HUDCrosshair = CreateWidget<UUserWidget>(GetWorld(), HUDCrosshairClass);
		if (HUDCrosshair)
		{
			HUDCrosshair->AddToViewport();
		}
	}

	if (MainHUDClass)
	{
		// 메인 HUD 위젯 생성 및 화면에 추가.
		MainHUD = CreateWidget<UUserWidget>(GetWorld(), MainHUDClass);
		if (MainHUD)
		{
			MainHUD->AddToViewport();
		}
	}	
	
	// 기본 무기를 생성해서 장착하고, 해당 무기 슬롯을 사용 중으로 표기 후, 무기의 보유 여부를 표시.
	EquipWeapon(SpawnDefaultWeapon());
	EquippedWeaponSlot(0);
	StoredWeapon();

	// 현재 장착 중인 무기의 슬롯 인덱스를 저장. (무기 교체에 사용)
	CurrentWeaponSlotIndex = EquippedWeapon->GetWeaponSlotIndex();

	/* DEBUG_LINE */
	UE_LOG(LogTemp, Warning, TEXT("Current Weapon Slot is: %d"), CurrentWeaponSlotIndex);
}

bool AFPShooterHero::GetCrosshairWorldPoint(FVector &CrosshairWorldLocation, FVector &CrosshairWorldDirection)
{
	// 화면의 크기를 구한다.
	FVector2D ViewportSize;

	if (GEngine)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
 
	// 크로스헤어의 화면 위치를 구한다.
	FVector2D CrosshairViewportPosition = FVector2D(ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f);

	// 크로스헤어의 월드 위치와 방향을 구한다.
	return UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairViewportPosition,
		CrosshairWorldLocation,
		CrosshairWorldDirection);
}

bool AFPShooterHero::GetFireTracePoint(const FVector& MuzzleSocketLocation, FVector& TraceLineLocation)
{	
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	// 크로스헤어의 월드 위치가 유효하면
	if (GetCrosshairWorldPoint(CrosshairWorldPosition, CrosshairWorldDirection))
	{
		// 크로스헤어의 위치에서, 크로스헤어의 월드 위치 + 월드 방향으로 라인 추적 생성.
		FHitResult ViewportTraceHit;	

		float WeaponDistance = EquippedWeapon->GetWeaponDistance();

		const FVector StartLocation = CrosshairWorldPosition;
		const FVector EndLocation = StartLocation + CrosshairWorldDirection * WeaponDistance;

		// 장착한 무기의 반동 수치와 반동 확산 범위를 구한다.
		float RecoilMultiplier = 0.0f;

		// 조준 상태라면 확산 수치를 감소, 조준 상태가 아니라면 확산 수치를 그대로 적용.
		bIsADS == true ? RecoilMultiplier = 
			EquippedWeapon->GetWeaponRecoil() * 0.25f : RecoilMultiplier = EquippedWeapon->GetWeaponRecoil();

		/* DEBUG_LINE */
		UE_LOG(LogTemp, Warning, TEXT("RecoilMultiplier: %f"), RecoilMultiplier);

		const float RecoilSpreadRangeX = EquippedWeapon->GetWeaponRecoilSpread().X;
		const float RecoilSpreadRangeY = EquippedWeapon->GetWeaponRecoilSpread().Y;

		// 총기의 반동 수치와 반동 확산 범위를 이용해 최종 착탄 위치를 결정.
		FVector2D BulletSpreadOffset = FVector2D(
			FMath::FRandRange(RecoilSpreadRangeX, RecoilSpreadRangeY), 
			FMath::FRandRange(RecoilSpreadRangeX, RecoilSpreadRangeY)) * 
			RecoilMultiplier;

		FVector ImpactLocation =
			EndLocation +
			FVector::CrossProduct(CrosshairWorldDirection, FVector::RightVector) * BulletSpreadOffset.X +
			FVector::CrossProduct(CrosshairWorldDirection, FVector::UpVector) * BulletSpreadOffset.Y;

		TraceLineLocation = ImpactLocation;

		GetWorld()->LineTraceSingleByChannel(
			ViewportTraceHit,
			StartLocation,
			ImpactLocation,
			ECollisionChannel::ECC_Visibility);
		
		// 라인에 충돌하면, 충돌 종료 위치를, 화면 추적의 충돌 위치와 동일하도록 설정.
		if (ViewportTraceHit.bBlockingHit)
		{
			TraceLineLocation = ViewportTraceHit.Location;
		}

		// 화면 추적(크로스헤어의 월드 위치와 방향으로 실행한 추적)이, 
		// 캐릭터와 추적 사이에 걸친 오브젝트를 체크하지 못해 
		// 이펙트가 오브젝트를 관통하여 표시되는 문제를 해결하기 위해
		// 총구 소켓을 기준으로 하는 무기 추적을 생성한다.
		FHitResult WeaponTraceHit;

		const FVector WeaponTraceStartLocation = MuzzleSocketLocation;
		const FVector WeaponTraceEndLocation = TraceLineLocation;

		// 추적된 액터의 Physical Material을 구하기 위해서 Query 파라미터 추가.
		FCollisionQueryParams WeaponTraceQueryParam = FCollisionQueryParams();

		// 자기 자신을 Query에서 제외하고, Physical Material을 반환하는 것을 허거한다.
		WeaponTraceQueryParam.AddIgnoredActor(this);
		WeaponTraceQueryParam.bReturnPhysicalMaterial = true;

		GetWorld()->LineTraceSingleByChannel(
			WeaponTraceHit,
			WeaponTraceStartLocation,
			WeaponTraceEndLocation,
			ECollisionChannel::ECC_Visibility,
			WeaponTraceQueryParam);

		/* DEBUG_LINE */
		DrawDebugLine(GetWorld(), WeaponTraceStartLocation, WeaponTraceEndLocation, FColor::Red, true, -1.0f, 0, 0.25f);

		// 라인에 충돌하면, 충돌 종료 위치를, 무기 추적의 충돌 위치와 동일하도록 설정.
		if (WeaponTraceHit.bBlockingHit)
		{
			TraceLineLocation = WeaponTraceHit.Location;	

			// 타격한 액터가 IFPShooterHitInterface 클래스를 상속받고 있는 지 확인.
			IFPShooterHitInterface* HitInterface = Cast< IFPShooterHitInterface>(WeaponTraceHit.GetActor());

			if (HitInterface)
			{
				// 피격된 액터의 라인 추적 피격에 대한 인터페이스 함수 호출.
				HitInterface->TraceHit_Implementation(WeaponTraceHit);

				// 피격된 액터가 적이라면 Damage를 처리.
				AFPShooterEnemy* HitEnemy = Cast<AFPShooterEnemy>(WeaponTraceHit.GetActor());

				if (HitEnemy)
				{
					HitEnemy->GetHealthComponent()->TakeDamage(50.0f);
				}
			}	
										
			/* DEBUG_LINE */
			DrawDebugPoint(GetWorld(), TraceLineLocation, 10.0f, FColor::Red, true, -1.0f, 0);			
		}
		return true;
	}
	return false;
}

void AFPShooterHero::StoreAmmo(EAmmoType AmmoType, int32 Amount)
{
	// 총알 타입에 따라, 총알이 최대 개수보다 적게 보관되어 있다면, 총알을 보관.
	switch (AmmoType)
	{
	case EAmmoType::EAT_Pistol:
		if (AmmoPack[AmmoType] < PistolAmmoMax)
		{
			int32 AmmoAmount = AmmoPack[AmmoType];
			AmmoAmount += Amount;			

			AmmoPack.Add(AmmoType, FMath::Clamp(AmmoAmount, 0, PistolAmmoMax));
		}		
		break;

	case EAmmoType::EAT_AR:
		if (AmmoPack[AmmoType] < ARAmmoMax)
		{
			int32 AmmoAmount = AmmoPack[AmmoType];
			AmmoAmount += Amount;

			AmmoPack.Add(AmmoType, FMath::Clamp(AmmoAmount, 0, ARAmmoMax));
		}
		break;

	case EAmmoType::EAT_Shotgun:
		if (AmmoPack[AmmoType] < ShotgunAmmoMax)
		{
			int32 AmmoAmount = AmmoPack[AmmoType];
			AmmoAmount += Amount;

			AmmoPack.Add(AmmoType, FMath::Clamp(AmmoAmount, 0, ShotgunAmmoMax));
		}
		break;

	case EAmmoType::EAT_GL:
		if (AmmoPack[AmmoType] < GLAmmoMax)
		{
			int32 AmmoAmount = AmmoPack[AmmoType];
			AmmoAmount += Amount;

			AmmoPack.Add(AmmoType, FMath::Clamp(AmmoAmount, 0, GLAmmoMax));
		}
		break;

	case EAmmoType::EAT_RL:
		if (AmmoPack[AmmoType] < RLAmmoMax)
		{
			int32 AmmoAmount = AmmoPack[AmmoType];
			AmmoAmount += Amount;

			AmmoPack.Add(AmmoType, FMath::Clamp(AmmoAmount, 0, RLAmmoMax));
		}
		break;

	case EAmmoType::EAT_SR:
		if (AmmoPack[AmmoType] < SRAmmoMax)
		{
			int32 AmmoAmount = AmmoPack[AmmoType];
			AmmoAmount += Amount;

			AmmoPack.Add(AmmoType, FMath::Clamp(AmmoAmount, 0, SRAmmoMax));
		}
		break;
	}

	/*DEBUG_LINE*/
	UE_LOG(LogTemp, Warning, TEXT("Ammo Amount: %d"), AmmoPack[AmmoType]);
}

// 질주 중에 스태미너를 감소/ 질주 상태가 아니면 스태미너를 증가
void AFPShooterHero::CalculateStamina()
{	
	if (bIsSprint)
	{
		// 질주 중이라면 Stamina를 감소
		if (Stamina <= 0.0f)
		{
			//Stamina의 0이하 감소 방지.
			Stamina = 0.0f;
			bIsSprint = false;
			GetCharacterMovement()->MaxWalkSpeed = HeroWalkSpeed;
		}
		else
		{
			Stamina -= 1.0;
		}
	}
	else
	{
		// 질주 중이 아니라면 Stamina를 복구.
		if (Stamina >= 100.0f)
		{
			//Stamina의 100이상 증가 방지.
			Stamina = 100.0f;
		}
		else
		{
			Stamina += 0.25;
		}
	}

	// 질주 상태에 따라 크로스헤어를 숨기거나 표시.
	HiddenCrosshair(bIsSprint);

	/* DEBUG_LINE */
	if (GEngine)
	{
		FString StaminaAmount = FString::Printf(TEXT("Stamina is: %f"), Stamina);

		GEngine->AddOnScreenDebugMessage(0, -1, FColor::Yellow, StaminaAmount);
	}
}

// 동적 크로스헤어의 확산 계산
void AFPShooterHero::CalcuateCrosshairSpreadMultiplier(float DeltaTime)
{
	// 점프 여부에 따른 크로스헤어 확산
	if (GetCharacterMovement()->IsFalling())
	{
		CrosshairSpreadJumpFactor = FMath::FInterpTo(
			CrosshairSpreadJumpFactor,
			2.25f,
			DeltaTime,
			2.25f);
	}
	else
	{
		CrosshairSpreadJumpFactor = FMath::FInterpTo(
			CrosshairSpreadJumpFactor,
			0.0f,
			DeltaTime,
			25.0f);	
	}

	// 격발 여부에 따른 크로스헤어 확산.
	if (bIsPullTheTrigger)
	{
		CrosshairSpreadShootingFactor = FMath::FInterpTo(
			CrosshairSpreadShootingFactor,
			0.5f,
			DeltaTime,
			30.0f);
	}
	else
	{
		CrosshairSpreadShootingFactor = FMath::FInterpTo(
			CrosshairSpreadShootingFactor,
			0.0f,
			DeltaTime,
			30.0f);
	}	

	// ADS 여부에 따른 크로스헤어 확산.
	if (bIsADS)
	{
		CrosshairSpreadADSFactor = FMath::FInterpTo(
			CrosshairSpreadADSFactor,
			2.0f,
			DeltaTime,
			30.0f);
	}
	else
	{
		CrosshairSpreadADSFactor = FMath::FInterpTo(
			CrosshairSpreadADSFactor,
			0.0f,
			DeltaTime,
			30.0f);
	}

	CrosshairSpreadMultiplier = CrosshairSpreadJumpFactor + CrosshairSpreadShootingFactor - CrosshairSpreadADSFactor;	
}

// 크로스헤어의 확산 최종 결과값을 구한다.
float AFPShooterHero::GetCrosshairSpreadMultiplier()
{
	return CrosshairSpreadMultiplier;
}

// 크로스헤어의 표시/ 숨김 처리.
void AFPShooterHero::HiddenCrosshair(bool bIsHidden)
{
	// 크로스헤어가 유효하면
	if (HUDCrosshair)
	{
		if (bIsHidden)
		{
			// 크로스헤어를 숨긴다.
			HUDCrosshair->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			// 크로스헤어를 표시한다.
			HUDCrosshair->SetVisibility(ESlateVisibility::Visible);
		}

	}
}

// 입력에 의한 ADS 상태 전환에 따른 카메라 FOV의 변화 보간 처리.(프레임마다 체크)
void AFPShooterHero::ADSInterpolationCamera(float DeltaTime)
{
	if (bIsADS)
	{
		ADSCameraView = FMath::FInterpTo(
			ADSCameraView,
			60.0f,
			DeltaTime,
			15.0f
		);

		FPShooterCamera->SetFieldOfView(ADSCameraView);
	}
	else
	{
		ADSCameraView = FMath::FInterpTo(
			ADSCameraView,
			90.0f,
			DeltaTime,
			15.0f
		);

		FPShooterCamera->SetFieldOfView(ADSCameraView);
	}	
}

// 입력에 의한 Crouch 상태 전환의 위치 보간 처리. (프레임마다 체크)
void AFPShooterHero::CrouchInterpolationStatus(float DeltaTime)
{
	if (GetCharacterMovement()->bWantsToCrouch)
	{
		FPShooterCameraView = FMath::FInterpTo(
			FPShooterCameraView,
			CrouchCameraHeight,
			DeltaTime,
			10.0f);
	}
	else
	{
		FPShooterCameraView = FMath::FInterpTo(
			FPShooterCameraView,
			BaseCameraHeight,
			DeltaTime,
			10.0f);
	}		

	FPShooterCamera->SetRelativeLocation(FVector(0.0f, 0.0f, FPShooterCameraView));
}

// 아이템을 획득.
void AFPShooterHero::PickUpItem(AFPShooterItem* Item)
{
	if (Item)
	{
		switch (Item->GetItemType())
		{
		case EItemType::EIT_Weapon:
			{
				AFPShooterWeapon* Weapon = Cast<AFPShooterWeapon>(Item);		

				PickUpWeapon(Weapon);
			}
			break;

		case EItemType::EIT_Ammo:
			{
				AFPShooterAmmo* Ammo = Cast<AFPShooterAmmo>(Item);

				PickUpAmmo(Ammo);
			}
			break;

		case EItemType::EIT_Health:
			UE_LOG(LogTemp, Warning, TEXT("Item Type is Health"));
			break;

		case EItemType::EIT_Shield:
			UE_LOG(LogTemp, Warning, TEXT("Item Type is Shield"));
			break;

		case EItemType::EIT_Stamina:
			UE_LOG(LogTemp, Warning, TEXT("Item Type is Stamina"));
			break;

		case EItemType::EIT_Energy:
			UE_LOG(LogTemp, Warning, TEXT("Item Type is Energy"));
			break;

		case EItemType::EIT_Coin:
			UE_LOG(LogTemp, Warning, TEXT("Item Type is Coin"));
			break;

		case EItemType::EIT_Material:
			UE_LOG(LogTemp, Warning, TEXT("Item Type is Material"));
			break;

		case EItemType::EIT_Memorial:
			UE_LOG(LogTemp, Warning, TEXT("Item Type is Memorial"));
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 무기 관리.

// 기본 무기 장착.
AFPShooterWeapon* AFPShooterHero::SpawnDefaultWeapon()
{
	if (DefaultWeaponClass)
	{
		AFPShooterWeapon* DefaultWeapon;

		DefaultWeapon = GetWorld()->SpawnActor<AFPShooterWeapon>(DefaultWeaponClass);

		if (DefaultWeapon)
		{
			// 기본 무기를 첫 번째 슬롯에 저장
			WeaponSlot.Insert(DefaultWeapon, 0);
			return DefaultWeapon;
		}
	}
	return nullptr;
}

// 무기를 획득.
void AFPShooterHero::PickUpWeapon(AFPShooterWeapon* Weapon)
{
	if (Weapon == nullptr)
	{
		return;
	}

	if (HasAleadyPickUpWeapon(Weapon))
	{
		// 무기가 보유 중인 총알 개수를, 영웅이 보유 중인 총알에 더하고, 액터를 제거.
		StoreAmmo(Weapon->GetAmmoType(), Weapon->GetCurrentAmmoAmount());
		Weapon->Destroy();
		return;
	}

	// 무기를 지정된 슬롯에 추가.
	WeaponSlot.Insert(Weapon, Weapon->GetWeaponSlotIndex());
	SwapWeapon(Weapon->GetWeaponSlotIndex());
	EquippedWeaponSlot(Weapon->GetWeaponSlotIndex());
	StoredWeapon();

	AFPShooterWeapon* HasWeapon = Weapon;

	/*DEBUG_LINE*/
	for (int i = 0; i < WeaponSlot.Num() - 1; i++)
	{
		HasWeapon = WeaponSlot[i];

		if (HasWeapon)
		{			
			UE_LOG(LogTemp, Warning, TEXT("Has Weapon is: %s"), *HasWeapon->GetName());
		}
	}

	CurrentWeaponSlotIndex = Weapon->GetWeaponSlotIndex();

	/* DEBUG_LINE */
	UE_LOG(LogTemp, Warning, TEXT("Weapon is: %s Add"), *Weapon->GetName());
	UE_LOG(LogTemp, Warning, TEXT("Current Weapon Slot is: %d"), CurrentWeaponSlotIndex);	
}

// 슬롯에 이미 무기가 등록되어 있는 지를 검사.
bool AFPShooterHero::HasAleadyPickUpWeapon(AFPShooterWeapon* PickUpWeapon)
{
	// 무기 슬롯에 동일한 무기를 이미 보유하고 있는지를 체크
	for (AFPShooterWeapon* Weapon : WeaponSlot)
	{
		if (Weapon)
		{
			/* DEBUG_LINE */
			UE_LOG(LogTemp, Warning, TEXT("Weapon Slot is: %d"), Weapon->GetWeaponSlotIndex());
			UE_LOG(LogTemp, Warning, TEXT("PickUpWeapon Slot is: %d"), PickUpWeapon->GetWeaponSlotIndex());

			// 무기 보관소가 비어있는 상태가 아니라면.
			if (Weapon->GetWeaponSlotIndex() == PickUpWeapon->GetWeaponSlotIndex())
			{
				/* DEBUG_LINE */
				UE_LOG(LogTemp, Warning, TEXT("Aleady Stored Weapon"));
				// 보유 중인 무기와 픽업된 무기가 동일하다면
				return true;
			}
		}
	}
	return false;
}

// 무기를 교체
void AFPShooterHero::SwapWeapon(int32 WeaponIndex)
{
	// 입력된 Index가 WeaponSlot의 범위를 벗어나면 작동하지 않는다.
	if (WeaponIndex < 0 || WeaponIndex >= WeaponSlot.Num())
	{
		return;
	}

	// 입력된 Index에 해당하는 무기 슬롯의 무기를 구한다.
	AFPShooterWeapon* NewWeapon = WeaponSlot[WeaponIndex];

	// 새로운 무기가 현재 장착된 무기와 동일하다면 작동하지 않는다.
	if (NewWeapon == EquippedWeapon)
	{
		return;
	}

	if (EquippedWeapon)
	{
		// 장착 중인 무기가 유효하면
		// 장착 중인 무기를 버리고, 버린 무기는 보이지 않도록 설정.
		EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		EquippedWeapon->GetItemMesh()->SetVisibility(false);
	}

	EquipWeapon(NewWeapon);
}

// 무기를 장작.
void AFPShooterHero::EquipWeapon(AFPShooterWeapon* Weapon)
{
	if (Weapon)
	{
		Weapon->GetItemCollision()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		Weapon->GetOverlapCollision()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

		FName GripSocketName;

		// 타입에 따라 적합한 그립 소켓에 무기가 장착되도록 소켓의 이름을 변경.
		switch (Weapon->GetWeaponType())
		{
		case EWeaponType::EWT_Pistol:
			GripSocketName = FName(TEXT("GripPistol"));
			break;

		case EWeaponType::EWT_AssultRifle:
			GripSocketName = FName(TEXT("GripAR"));
			break;

		case EWeaponType::EWT_Shotgun:
			GripSocketName = FName(TEXT("GripShotgun"));
			break;

		case EWeaponType::EWT_GrenadeLauncher:
			GripSocketName = FName(TEXT("GripGL"));
			break;

		case EWeaponType::EWT_RocketLauncher:
			GripSocketName = FName(TEXT("GripRL"));
			break;

		case EWeaponType::EWT_SniperRifle:
			GripSocketName = FName(TEXT("GripSR"));
			break;
		}	

		const USkeletalMeshSocket* GripSocket = FPShooterMesh->GetSocketByName(GripSocketName);

		if (GripSocket)
		{
			GripSocket->AttachActor(Weapon, FPShooterMesh);
		}

		EquippedWeapon = Weapon;
		// 새롭게 장착된 무기를 보이도록 설정.
		EquippedWeapon->GetItemMesh()->SetVisibility(true);
	}
}

// 발사 타이머 시작.
void AFPShooterHero::StartWeaponFire()
{
	/* DEBUG_LINE */
	UE_LOG(LogTemp, Warning, TEXT("Firing is %s"), bIsFiring ? TEXT("TRUE") : TEXT("FALSE"));

	if (EquippedWeapon == nullptr)
	{
		// 장착된 무기가 유효하지 않으면 아무런 작동도 하지 않는다.
		return;
	}

	if (EquippedWeapon->GetCurrentAmmoAmount() < 1)
	{
		// 장착된 무기에 남아있는 총알이 없다면 강제로 Reload 실행.
		bIsFiring = false;

		GetWorldTimerManager().ClearTimer(WeaponFireTimer);
		StartReload();
	}

	if (!bIsFiring && !bIsReload && EquippedWeapon->GetCurrentAmmoAmount() > 0)
	{
		// 무기가 발사 중이라는 것을 설정.
		bIsFiring = true;

		// 총알을 1개 소모한다.
		EquippedWeapon->ConsumeAmmo(1);

		// 무기의 발사 타입에 따라 적합한 발사 처리 함수를 호출.
		switch (EquippedWeapon->GetWeaponFireType())
		{
		case EWeaponFireType::EWFT_Manual:
			GetWorldTimerManager().SetTimer(WeaponFireTimer, this, &AFPShooterHero::FinishFire, EquippedWeapon->GetFireRate(), true);
			WeaponFire();
			PlayFireMontage();

			/* DEBUG_LINE */
			UE_LOG(LogTemp, Warning, TEXT("Weapon Fire Type is Manual"));
			break;

		case EWeaponFireType::EWFT_Auto:
			GetWorldTimerManager().SetTimer(WeaponFireTimer, this, &AFPShooterHero::FinishAutoFire, EquippedWeapon->GetFireRate(), true);
			WeaponFire();
			PlayFireMontage();

			/* DEBUG_LINE */
			UE_LOG(LogTemp, Warning, TEXT("Weapon Fire Type is Auto"));
			break;

		case EWeaponFireType::EWFT_Spread:
			GetWorldTimerManager().SetTimer(WeaponFireTimer, this, &AFPShooterHero::FinishFire, EquippedWeapon->GetFireRate(), true);
			WeaponSpreadFire();
			PlayFireMontage();

			/* DEBUG_LINE */
			UE_LOG(LogTemp, Warning, TEXT("Weapon Fire Type is Spread"));
			break;

		case EWeaponFireType::EWFT_Projectile:
			GetWorldTimerManager().SetTimer(WeaponFireTimer, this, &AFPShooterHero::FinishFire, EquippedWeapon->GetFireRate(), true);
			WeaponProjectileFire();
			PlayFireMontage();

			/* DEBUG_LINE */
			UE_LOG(LogTemp, Warning, TEXT("Weapon Fire Type is Projectile"));
			break;
		}
	}
}

// 수동 무기의 발사 타이머 종료.
void AFPShooterHero::FinishFire()
{
	bIsFiring = false;

	GetWorldTimerManager().ClearTimer(WeaponFireTimer);

	/* DEBUG_LINE */
	UE_LOG(LogTemp, Warning, TEXT("Firing is End"));

	if (EquippedWeapon->GetCurrentAmmoAmount() < 1)
	{
		// 장착된 무기에 남아있는 총알이 없다면 강제로 Reload 실행.		
		StartReload();
	}
}

// 자동 무기의 발사 타이머 종료.
void AFPShooterHero::FinishAutoFire()
{
	bIsFiring = false;

	if (EquippedWeapon->GetCurrentAmmoAmount() < 1)
	{
		// 장착된 무기에 남아있는 총알이 없다면 강제로 Reload 실행.
		GetWorldTimerManager().ClearTimer(WeaponFireTimer);
		StartReload();
	}

	if (bIsPullTheTrigger && !bIsReload && EquippedWeapon->GetCurrentAmmoAmount() > 0)
	{
		StartWeaponFire();
	}

	/* DEBUG_LINE */
	UE_LOG(LogTemp, Warning, TEXT("Firing is End"));
}

// 발사 몽타주 재생.
void AFPShooterHero::PlayFireMontage()
{
	// 발사 애니메이션 인스턴스 획득.
	UAnimInstance* FireAnimInstance = FPShooterMesh->GetAnimInstance();

	// 몽타주 섹션의 이름을 저장할 변수 설정 (일반)
	FName FireMontageSectioName;

	// 몽타주 섹션의 이름을 저장할 변수 설정 (ADS)
	FName ADSFireMontageSectionName;

	// 무기 타입에 따라  발사 몽타주 섹션 이름을 적합하도록 설정.
	// 4.GrenadeLauncher -> GLauncherFire
	// 5.RocketLauncher -> RLauncherFire
	// 6.SniperRifle -> SRifleFire/ SRifleADSFire 	
	switch (EquippedWeapon->GetWeaponType())
	{
	case EWeaponType::EWT_Pistol:
		FireMontageSectioName = FName(TEXT("PistolFire"));
		ADSFireMontageSectionName = FName(TEXT("PistolADSFire"));

		/* DEBUG_LINE */
		UE_LOG(LogTemp, Warning, TEXT("Fire Montage Section Name: %s"), *FireMontageSectioName.ToString());
		UE_LOG(LogTemp, Warning, TEXT("Fire ADS Montage Section Name: %s"), *ADSFireMontageSectionName.ToString());
		break;

	case EWeaponType::EWT_AssultRifle:
		FireMontageSectioName = FName(TEXT("RifleFire"));
		ADSFireMontageSectionName = FName(TEXT("RifleADSFire"));

		/* DEBUG_LINE */
		UE_LOG(LogTemp, Warning, TEXT("Fire Montage Section Name: %s"), *FireMontageSectioName.ToString());
		UE_LOG(LogTemp, Warning, TEXT("Fire ADS Montage Section Name: %s"), *ADSFireMontageSectionName.ToString());
		break;

	case EWeaponType::EWT_Shotgun:
		FireMontageSectioName = FName(TEXT("RifleFire"));
		ADSFireMontageSectionName = FName(TEXT("RifleADSFire"));

		break;

	case EWeaponType::EWT_GrenadeLauncher:
		FireMontageSectioName = FName(TEXT("RifleFire"));
		ADSFireMontageSectionName = FName(TEXT("RifleADSFire"));

		break;

	case EWeaponType::EWT_RocketLauncher:
		FireMontageSectioName = FName(TEXT("RifleFire"));
		ADSFireMontageSectionName = FName(TEXT("RifleADSFire"));

		break;

	case EWeaponType::EWT_SniperRifle:
		FireMontageSectioName = FName(TEXT("RifleFire"));
		ADSFireMontageSectionName = FName(TEXT("RifleADSFire"));

		break;
	}

	// 조준 상태 여부가 유효하면.
	if (bIsADS)
	{
		if (AimFireMontage && FireAnimInstance)
		{
			FireAnimInstance->Montage_Play(AimFireMontage);
			FireAnimInstance->Montage_JumpToSection(ADSFireMontageSectionName);
		}
	}
	else
	{
		if (FireMontage && FireAnimInstance)
		{
			FireAnimInstance->Montage_Play(FireMontage);
			FireAnimInstance->Montage_JumpToSection(FireMontageSectioName);
		}
	}
}

// 무기 발사 처리
void AFPShooterHero::WeaponFire()
{
	const FVector MuzzleSocketLocation = EquippedWeapon->GetMuzzleSocket()->GetSocketLocation(EquippedWeapon->GetItemMesh());
	FVector TraceLineEndPoint;

	// 대상에 대한 타격 여부를 검사.
	bool bIsGetFireTracePoint = GetFireTracePoint(MuzzleSocketLocation, TraceLineEndPoint);

	if (bIsGetFireTracePoint)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), EquippedWeapon->GetMuzzleFlash(), MuzzleSocketLocation);
		UGameplayStatics::PlaySoundAtLocation(this, EquippedWeapon->GetFireSound(), MuzzleSocketLocation);
	}	
}

void AFPShooterHero::WeaponSpreadFire()
{
	const FVector MuzzleSocketLocation = EquippedWeapon->GetMuzzleSocket()->GetSocketLocation(EquippedWeapon->GetItemMesh());
	FVector TraceLineEndPoint;

	/* DEBUG_LINE */
	UE_LOG(LogTemp, Warning, TEXT("Ray is: %d"), EquippedWeapon->GetRayNums());

	for (int32 Ray = 0; Ray < EquippedWeapon->GetRayNums(); Ray++)
	{
		// 대상에 대한 타격 여부를 검사.
		bool bIsGetFireMultiTracePoint = GetFireTracePoint(MuzzleSocketLocation, TraceLineEndPoint);

		if (bIsGetFireMultiTracePoint)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), EquippedWeapon->GetMuzzleFlash(), MuzzleSocketLocation);
			UGameplayStatics::PlaySoundAtLocation(this, EquippedWeapon->GetFireSound(), MuzzleSocketLocation);
		}
	}	
}

void AFPShooterHero::WeaponProjectileFire()
{
	const FTransform MuzzleSocketTrans = EquippedWeapon->GetMuzzleSocket()->GetSocketTransform(EquippedWeapon->GetItemMesh());

	const FVector MuzzleSocketLocation = MuzzleSocketTrans.GetLocation();
	const FRotator MuzzleSocketRotation = FRotator(MuzzleSocketTrans.GetRotation());

	AFPShooterProjectile* LaunchProjectile;

	LaunchProjectile = EquippedWeapon->SpawnProjectile(MuzzleSocketLocation, MuzzleSocketRotation);

	if (LaunchProjectile)
	{
		LaunchProjectile->GetProjectileMesh()->AddImpulse(
			GetActorForwardVector() *
			EquippedWeapon->GetLaunchForce());	
	}
}

// 무기 슬롯에서 이전 무기로 교체
void AFPShooterHero::SwitchPrevWeapon()
{	
	// 현재 무기 슬롯 인덱스에서 1을 감소.
	int32 SlotNum = CurrentWeaponSlotIndex - 1;

	if (SlotNum < 0)
	{
		// 슬롯 인덱스가 0보다 작게 되면.
		// 가장 마지막 슬롯의 무기로 교체. (순환 교체)
		CurrentWeaponSlotIndex = WeaponSlot.Num() - 1;
	}
	else
	{
		if (WeaponSlot[SlotNum])
		{
			// 해당 슬롯에 저장된 무기가 유효하다면, 
			// 최소 인덱스에서 최대 인덱스 사이로 제한된 슬롯 인덱스를 저장.
			CurrentWeaponSlotIndex = FMath::Clamp(SlotNum, 0, WeaponSlot.Num() - 1);
		}
	}

	/* DEBUG_LINE */
	UE_LOG(LogTemp, Warning, TEXT("Current Weapon Slot is: %d"), CurrentWeaponSlotIndex);
}

// 무기 슬롯에서 다음 무기로 교체
void AFPShooterHero::SwitchNextWeapon()
{	
	// 현재의 무기 슬롯 인덱스에서 1을 증가.
	int32 SlotNum = CurrentWeaponSlotIndex + 1;

	if (SlotNum > WeaponSlot.Num() - 1)
	{
		// 슬롯 인덱스가, 무기 슬롯의 크기보다 크게 되면.
		// 맨 처음 슬롯의 무기로 교체. (순환 교체)
		CurrentWeaponSlotIndex = 0;
	}
	else
	{
		if (WeaponSlot[SlotNum])
		{
			// 해당 슬롯에 저장된 무기가 유효하다면, 
			// 최소 인덱스에서 최대 인덱스 사이로 제한된 슬롯 인덱스를 저장.
			CurrentWeaponSlotIndex = FMath::Clamp(SlotNum, 0, WeaponSlot.Num() - 1);
		}
	}
	
	/* DEBUG_LINE */
	UE_LOG(LogTemp, Warning, TEXT("Current Weapon Slot is: %d"), CurrentWeaponSlotIndex);
}

// 슬롯에 저장된 무기가 있다면, 해당 무기 Thumbnail을 활성화하는 이벤트 호출.
void AFPShooterHero::StoredWeapon()
{
	for (AFPShooterWeapon* Weapon : WeaponSlot)
	{
		if (Weapon)
		{
			PickedUpWeaponSlot(Weapon->GetWeaponSlotIndex());
		}
	}
}

// 픽업한 탄약을 보관.
void AFPShooterHero::PickUpAmmo(AFPShooterAmmo* Ammo)
{
	if (Ammo)
	{
		if (AmmoPack.Find(Ammo->GetAmmoType()))
		{
			// 픽업할 탄약과 동일한 타입의 탄약을 보유 중이라면, 픽업할 탄약과 보유 중인 탄약을 합산해서, 보유 중인 탄약의
			// 수량에 반영.
			int32 AmmoAmount = Ammo->GetItemAmount() + AmmoPack[Ammo->GetAmmoType()];
			AmmoPack[Ammo->GetAmmoType()] = AmmoAmount;

			// 액터를 제거.
			Ammo->Destroy();
		}
	}
}

// 게임 업데이트
void AFPShooterHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CalculateStamina();

	CalcuateCrosshairSpreadMultiplier(DeltaTime);

	ADSInterpolationCamera(DeltaTime);

	CrouchInterpolationStatus(DeltaTime);

	/* DEBUG_LINE */
	FString SpreadMultiplier = FString::Printf(TEXT("Crosshair Multiplier is: %f"), CrosshairSpreadMultiplier);
	FString AmmoPackAmount = FString::Printf(TEXT("Ammo Pack's Ammo Amount is: %d"), AmmoPack[EquippedWeapon->GetAmmoType()]);
	FString AmmoAmount = FString::Printf(TEXT("Current Ammo Amount is: %d"), EquippedWeapon->GetCurrentAmmoAmount());

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, -1, FColor::Blue, SpreadMultiplier);
		GEngine->AddOnScreenDebugMessage(3, -1.0f, FColor::White, AmmoPackAmount);
		GEngine->AddOnScreenDebugMessage(6, -1.0f, FColor::Purple, AmmoAmount);
	}
}

void AFPShooterHero::StartReload()
{
	// 재징전 상태 여부가 유효하면.
	if (bIsReload)
	{
		//재장전 중에는 또 다시 재장전이 실행되지 않도록 방지.
		return;
	}

	// 보유 중인 총알 중에, 장착된 무기의 총알 타입과 동일한 타입의 총알의 보유가 유효하면.
	if (AmmoPack.Contains(EquippedWeapon->GetAmmoType()))
	{
		if (AmmoPack[EquippedWeapon->GetAmmoType()] <= 0)
		{
			/* DEBUG_LINE */
			UE_LOG(LogTemp, Warning, TEXT("No Ammo!!"));
			// 보유 중인 총알이 없으면, 아무런 동작도 하지 않는다.
			return;
		}

		bIsReload = true;

		/* DEBUG_LINE */
		UE_LOG(LogTemp, Warning, TEXT("Reload Start"));

		// 보충할 총알의 개수
		int32 ReloadAmmoAmount = 0;

		// 보유 중인 총알의 개수가, 장착된 무기의 비어있는 탄창의 공간보다 많을 경우.
		if (AmmoPack[EquippedWeapon->GetAmmoType()] > EquippedWeapon->GetEmptyMagazineCapacity())
		{
			// 장전할 총알의 개수를 구한다.
			ReloadAmmoAmount = EquippedWeapon->GetEmptyMagazineCapacity();
			// 장전 후, 남은 총알의 개수를 구한다.
			int32 RemainAmmoAmount = AmmoPack[EquippedWeapon->GetAmmoType()] - EquippedWeapon->GetEmptyMagazineCapacity();
			// 보유 중인 총알의 개수를 장전 후, 남은 총알의 개수로 갱신.
			AmmoPack.Add(EquippedWeapon->GetAmmoType(), RemainAmmoAmount);

			/*DEBUG_LINE*/
			UE_LOG(LogTemp, Warning, TEXT("A: Reload Ammo Amount is: %d"), ReloadAmmoAmount);
			UE_LOG(LogTemp, Warning, TEXT("A: Remain Ammo Amount is: %d"), RemainAmmoAmount);
		}

		// 보유 중이 총알의 개수가, 장착된 무기의 비어있는 탄창의 공간보다 적을 경우.
		if (AmmoPack[EquippedWeapon->GetAmmoType()] <= EquippedWeapon->GetEmptyMagazineCapacity())
		{
			// 장전할 총알의 개수를 구한다.
			ReloadAmmoAmount = AmmoPack[EquippedWeapon->GetAmmoType()];
			// 장전 후,남은 총알의 개수를 0으로 설정.
			int32 RemainAmmoAmount = 0;
			// 보유 중인 총알의 개수를 장전 후, 남은 총알의 개수로 갱신.
			AmmoPack.Add(EquippedWeapon->GetAmmoType(), RemainAmmoAmount);

			/*DEBUG_LINE*/
			UE_LOG(LogTemp, Warning, TEXT("B: Reload Ammo Amount is: %d"), ReloadAmmoAmount);
			UE_LOG(LogTemp, Warning, TEXT("B: Remain Ammo Amount is: %d"), RemainAmmoAmount);
		}

		EquippedWeapon->ChargeAmmo(ReloadAmmoAmount);

		// 재장전 애니메이션 재생.
		UAnimInstance* ReloadAnimInstance = FPShooterMesh->GetAnimInstance();

		if (ReloadMontage && ReloadAnimInstance)
		{
			ReloadAnimInstance->Montage_Play(ReloadMontage);
			ReloadAnimInstance->Montage_JumpToSection(FName("WeaponReload"));
			ReloadTime = ReloadMontage->GetSectionLength(ReloadMontage->GetSectionIndex(FName("WeaponReload")));
			GetWorldTimerManager().SetTimer(ReloadTimer, this, &AFPShooterHero::FinishReload, ReloadTime);
		}

		/* DEBUG_LIN E*/
		UE_LOG(LogTemp, Warning, TEXT("B: Reload Time is: %f"), ReloadTime);
	}
}

void AFPShooterHero::FinishReload()
{
	bIsReload = false;

	UE_LOG(LogTemp, Warning, TEXT("Reload Finish"));
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 플레이어 입력으로 영웅 제어.

// 영웅을 앞으로/ 뒤로 이동시키기.
void AFPShooterHero::HeroMoveForward(float AxisValue)
{
	if (Controller)
	{
		AddMovementInput(GetActorForwardVector(), AxisValue);
	}	
}

// 영웅을 왼쪽으로/ 오른쪽으로 이동시키기.
void AFPShooterHero::HeroMoveRight(float AxisValue)
{
	if (Controller)
	{
		AddMovementInput(GetActorRightVector(), AxisValue);
	}	
}

// 영웅의 시야를 위로/ 아래로 회전시키기. (마우스 입력)
void AFPShooterHero::HeroLookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

// 영웅의 시야를 위로/ 아래로 회전시키기. (게임패드 입력)
void AFPShooterHero::HeroLookUpGamepad(float AxisValue)
{
	AddControllerPitchInput(AxisValue * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

// 영웅의 시야를 왼쪽으로/ 오른쪽으로 회전시키기. (마우스 입력)
void AFPShooterHero::HeroTurn(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

// 영웅의 시야를 왼쪽으로/ 오른쪽으로 회전시키기. (게임패드 입력)
void AFPShooterHero::HeroTurnGamepad(float AxisValue)
{
	AddControllerYawInput(AxisValue * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFPShooterHero::JumpPressed()
{
	// Crouch 상태가 유효하면
	if (GetCharacterMovement()->IsCrouching())
	{
		// Crouch 상태 해제.
		UnCrouch();
	}

	Jump();
}

void AFPShooterHero::JumpReleased()
{
	if (GetCharacterMovement()->IsFalling())
	{
		StopJumping();
	}	
}

// 영웅이 질주하기.
void AFPShooterHero::SpirntPressed()
{
	if (GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0f)
	{
		// 이동 중이 아니면 질주할 수 없다.
		if (bIsReload || GetCharacterMovement()->IsFalling())
		{
			//Reload 상태이거나, 점프 중이라면 질주 불가.
			return;
		}

		if (bIsADS)
		{
			// ADS 상태라면, ADS 상태 해제.
			bIsADS = false;
		}

		if (GetCharacterMovement()->IsCrouching())
		{
			UnCrouch();
		}

		// 질주 상태를 true로 변경하고, 영웅의 이동 속도를 2배 증가.
		bIsSprint = true;
		GetCharacterMovement()->MaxWalkSpeed = HeroWalkSpeed * 2.0f;
	}	
}

// 영웅의 질주 멈추기.
void AFPShooterHero::SprintReleased()
{
	if (bIsSprint)
	{
		bIsSprint = false;
		GetCharacterMovement()->MaxWalkSpeed = HeroWalkSpeed;
	}	
}

// 트리거 버튼을 입력하여 발사하기.
void AFPShooterHero::TriggerPressed()
{
	if (bIsReload)
	{
		// Reload 상태에서는 격발 불가.
		return;
	}

	// 트리거 입력 유효.
	bIsPullTheTrigger = true;

	// 무기 발사를 실행.
	StartWeaponFire();	
}

// 트리거 버튼의 입력을 해제하여 발사 멈추기.
void AFPShooterHero::TriggerReleased()
{
	bIsPullTheTrigger = false;
}

// ADS 모드 입력.
void AFPShooterHero::ADSPressed()
{
	if (bIsReload || bIsSprint)
	{
		// Reload 또는 Sprint 상태에서는 ADS 실행 불가.
		return;
	}

	// 무기의 ADS 모드 실행 여부에 따른 ADS 실행.
	if (!EquippedWeapon->GetPossibleADS())
	{
		return;
	}
	
	bIsADS = true;
	GetCharacterMovement()->MaxWalkSpeed = HeroWalkSpeed * 0.75;		
}

// ADS 모드 해제
void AFPShooterHero::ADSReleased()
{
	if (bIsADS)
	{
		bIsADS = false;
		GetCharacterMovement()->MaxWalkSpeed = HeroWalkSpeed;
	}	
}

void AFPShooterHero::CrouchPressed()
{
	// 점프 중이면 Crouch 불가.
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	// Crouch 상태가 유효하면.
	if (GetCharacterMovement()->IsCrouching())
	{
		// Crouch 상태를 해제하고, 카메라의 높이를 복구.	
		UnCrouch();
	}
	else
	{	
		// Crouch 상태로 전환하고, 카메라의 높이를 Crouch 상태로 변경.
		Crouch();
	}	
}

void AFPShooterHero::ReloadPressed()
{
	if (bIsSprint || bIsPullTheTrigger)
	{
		// Sprint 또는 격발 상태라면 Reload 실행 불가.
		return;
	}

	if (bIsADS)
	{
		// ADS 상태라면, ADS 상태 해제.
		bIsADS = false;
	}
	
	//Reload를 실행한다.
	StartReload();
}

// 현재 장착한 무기의 이전 슬롯에 등록된 무기로 교체.
void AFPShooterHero::SwitchPrevWeaponPressed()
{
	// 이전 슬롯의 무기로 교체하고, 교체한 무기 슬롯 썸네일의 색상을 변경.
	SwitchPrevWeapon();
	SwapWeapon(CurrentWeaponSlotIndex);
	EquipWeapon(WeaponSlot[CurrentWeaponSlotIndex]);
	EquippedWeaponSlot(EquippedWeapon->GetWeaponSlotIndex());	
}

// 현재 장착한 무기의 다음 슬롯에 등록된 무기로 교체.
void AFPShooterHero::SwitchNextWeaponPressed()
{
	// 다음 슬롯의 무기로 교체하고, 교체한 무기 슬롯 썸네일의 색상을 변경.
	SwitchNextWeapon();
	SwapWeapon(CurrentWeaponSlotIndex);
	EquipWeapon(WeaponSlot[CurrentWeaponSlotIndex]);
	EquippedWeaponSlot(EquippedWeapon->GetWeaponSlotIndex());
}

void AFPShooterHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 축(Axis) 입력 함수 바인딩
	PlayerInputComponent->BindAxis(FName("Move Forward / Backward"), this, &AFPShooterHero::HeroMoveForward);
	PlayerInputComponent->BindAxis(FName("Move Right / Left"), this, &AFPShooterHero::HeroMoveRight);
	PlayerInputComponent->BindAxis(FName("Look Up / Down Mouse"), this, &AFPShooterHero::HeroLookUp);
	PlayerInputComponent->BindAxis(FName("Look Up / Down Gamepad"), this, &AFPShooterHero::HeroLookUpGamepad);
	PlayerInputComponent->BindAxis(FName("Turn Right / Left Mouse"), this, &AFPShooterHero::HeroTurn);
	PlayerInputComponent->BindAxis(FName("Turn Right / Left Gamepad"), this, &AFPShooterHero::HeroTurnGamepad);

	// 액션(Button) 입력 함수 바인딩
	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &AFPShooterHero::JumpPressed);
	PlayerInputComponent->BindAction(FName("Jump"), IE_Released, this, &AFPShooterHero::JumpReleased);
	PlayerInputComponent->BindAction(FName("Fire"), IE_Pressed, this, &AFPShooterHero::TriggerPressed);
	PlayerInputComponent->BindAction(FName("Fire"), IE_Released, this, &AFPShooterHero::TriggerReleased);
	PlayerInputComponent->BindAction(FName("Sprint"), IE_Pressed, this, &AFPShooterHero::SpirntPressed);
	PlayerInputComponent->BindAction(FName("Sprint"), IE_Released, this, &AFPShooterHero::SprintReleased);
	PlayerInputComponent->BindAction(FName("ADS"), IE_Pressed, this, &AFPShooterHero::ADSPressed);
	PlayerInputComponent->BindAction(FName("ADS"), IE_Released, this, &AFPShooterHero::ADSReleased);
	PlayerInputComponent->BindAction(FName("Crouch"), IE_Pressed, this, &AFPShooterHero::CrouchPressed);
	PlayerInputComponent->BindAction(FName("Reload"), IE_Pressed, this, &AFPShooterHero::ReloadPressed);
	PlayerInputComponent->BindAction(FName("PrevWeapon"), IE_Pressed, this, &AFPShooterHero::SwitchPrevWeaponPressed);
	PlayerInputComponent->BindAction(FName("NextWeapon"), IE_Pressed, this, &AFPShooterHero::SwitchNextWeaponPressed);
}
