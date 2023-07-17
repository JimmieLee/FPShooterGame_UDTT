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
	// �Ѿ� ���� ����
	PistolAmmoMax(48),
	ARAmmoMax(128),
	ShotgunAmmoMax(28),
	GLAmmoMax(24),
	RLAmmoMax(4),
	SRAmmoMax(28),
	// ũ�ν���� Ȯ�� ����ġ
	CrosshairSpreadMultiplier(0.0f),
	CrosshairSpreadJumpFactor(0.0f),
	CrosshairSpreadADSFactor(0.0f),
	CrosshairSpreadShootingFactor(0.0f)
{	
	// �÷��̾� ī�޶� ������Ʈ ����.
	FPShooterCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	if (FPShooterCamera)
	{
		FPShooterCamera->SetupAttachment(RootComponent);
		FPShooterCamera->SetRelativeLocation(FVector(0.0f, 0.0f, BaseCameraHeight));
		FPShooterCamera->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	}

	// FPS ĳ���� �޽� ������Ʈ ����.
	FPShooterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShooterMesh"));
	if (FPShooterMesh)
	{
		FPShooterMesh->SetupAttachment(FPShooterCamera);
		FPShooterMesh->SetRelativeLocation(FVector(10.0f, 0.0f, BaseMeshHeight));
		FPShooterMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		FPShooterMesh->CastShadow = 0;
	}

	// ī�޶� ��Ʈ�ѷ��� ȸ���� ����ϵ��� ����.
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Crouch ����
	GetCharacterMovement()->bWantsToCrouch = true;
 }

void AFPShooterHero::BeginPlay()
{
	Super::BeginPlay();		
	
	// ���� ź�� �ʱ�ȭ 
	AmmoPack.Add(EAmmoType::EAT_Pistol, 0);
	AmmoPack.Add(EAmmoType::EAT_AR, 0);
	AmmoPack.Add(EAmmoType::EAT_Shotgun, 0);
	AmmoPack.Add(EAmmoType::EAT_GL, 0);
	AmmoPack.Add(EAmmoType::EAT_RL, 0);
	AmmoPack.Add(EAmmoType::EAT_SR, 0);
	
	// �⺻ ����(�ǽ���)�� ���� ź�� ���� ���� ����.
	StoreAmmo(EAmmoType::EAT_Pistol, 10);
	
	if (HUDCrosshairClass)
	{
		// ũ�ν���� ���� ���� �� ȭ�鿡 �߰�.
		HUDCrosshair = CreateWidget<UUserWidget>(GetWorld(), HUDCrosshairClass);
		if (HUDCrosshair)
		{
			HUDCrosshair->AddToViewport();
		}
	}

	if (MainHUDClass)
	{
		// ���� HUD ���� ���� �� ȭ�鿡 �߰�.
		MainHUD = CreateWidget<UUserWidget>(GetWorld(), MainHUDClass);
		if (MainHUD)
		{
			MainHUD->AddToViewport();
		}
	}	
	
	// �⺻ ���⸦ �����ؼ� �����ϰ�, �ش� ���� ������ ��� ������ ǥ�� ��, ������ ���� ���θ� ǥ��.
	EquipWeapon(SpawnDefaultWeapon());
	EquippedWeaponSlot(0);
	StoredWeapon();

	// ���� ���� ���� ������ ���� �ε����� ����. (���� ��ü�� ���)
	CurrentWeaponSlotIndex = EquippedWeapon->GetWeaponSlotIndex();

	/* DEBUG_LINE */
	UE_LOG(LogTemp, Warning, TEXT("Current Weapon Slot is: %d"), CurrentWeaponSlotIndex);
}

bool AFPShooterHero::GetCrosshairWorldPoint(FVector &CrosshairWorldLocation, FVector &CrosshairWorldDirection)
{
	// ȭ���� ũ�⸦ ���Ѵ�.
	FVector2D ViewportSize;

	if (GEngine)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
 
	// ũ�ν������ ȭ�� ��ġ�� ���Ѵ�.
	FVector2D CrosshairViewportPosition = FVector2D(ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f);

	// ũ�ν������ ���� ��ġ�� ������ ���Ѵ�.
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

	// ũ�ν������ ���� ��ġ�� ��ȿ�ϸ�
	if (GetCrosshairWorldPoint(CrosshairWorldPosition, CrosshairWorldDirection))
	{
		// ũ�ν������ ��ġ����, ũ�ν������ ���� ��ġ + ���� �������� ���� ���� ����.
		FHitResult ViewportTraceHit;	

		float WeaponDistance = EquippedWeapon->GetWeaponDistance();

		const FVector StartLocation = CrosshairWorldPosition;
		const FVector EndLocation = StartLocation + CrosshairWorldDirection * WeaponDistance;

		// ������ ������ �ݵ� ��ġ�� �ݵ� Ȯ�� ������ ���Ѵ�.
		float RecoilMultiplier = 0.0f;

		// ���� ���¶�� Ȯ�� ��ġ�� ����, ���� ���°� �ƴ϶�� Ȯ�� ��ġ�� �״�� ����.
		bIsADS == true ? RecoilMultiplier = 
			EquippedWeapon->GetWeaponRecoil() * 0.25f : RecoilMultiplier = EquippedWeapon->GetWeaponRecoil();

		/* DEBUG_LINE */
		UE_LOG(LogTemp, Warning, TEXT("RecoilMultiplier: %f"), RecoilMultiplier);

		const float RecoilSpreadRangeX = EquippedWeapon->GetWeaponRecoilSpread().X;
		const float RecoilSpreadRangeY = EquippedWeapon->GetWeaponRecoilSpread().Y;

		// �ѱ��� �ݵ� ��ġ�� �ݵ� Ȯ�� ������ �̿��� ���� ��ź ��ġ�� ����.
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
		
		// ���ο� �浹�ϸ�, �浹 ���� ��ġ��, ȭ�� ������ �浹 ��ġ�� �����ϵ��� ����.
		if (ViewportTraceHit.bBlockingHit)
		{
			TraceLineLocation = ViewportTraceHit.Location;
		}

		// ȭ�� ����(ũ�ν������ ���� ��ġ�� �������� ������ ����)��, 
		// ĳ���Ϳ� ���� ���̿� ��ģ ������Ʈ�� üũ���� ���� 
		// ����Ʈ�� ������Ʈ�� �����Ͽ� ǥ�õǴ� ������ �ذ��ϱ� ����
		// �ѱ� ������ �������� �ϴ� ���� ������ �����Ѵ�.
		FHitResult WeaponTraceHit;

		const FVector WeaponTraceStartLocation = MuzzleSocketLocation;
		const FVector WeaponTraceEndLocation = TraceLineLocation;

		// ������ ������ Physical Material�� ���ϱ� ���ؼ� Query �Ķ���� �߰�.
		FCollisionQueryParams WeaponTraceQueryParam = FCollisionQueryParams();

		// �ڱ� �ڽ��� Query���� �����ϰ�, Physical Material�� ��ȯ�ϴ� ���� ����Ѵ�.
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

		// ���ο� �浹�ϸ�, �浹 ���� ��ġ��, ���� ������ �浹 ��ġ�� �����ϵ��� ����.
		if (WeaponTraceHit.bBlockingHit)
		{
			TraceLineLocation = WeaponTraceHit.Location;	

			// Ÿ���� ���Ͱ� IFPShooterHitInterface Ŭ������ ��ӹް� �ִ� �� Ȯ��.
			IFPShooterHitInterface* HitInterface = Cast< IFPShooterHitInterface>(WeaponTraceHit.GetActor());

			if (HitInterface)
			{
				// �ǰݵ� ������ ���� ���� �ǰݿ� ���� �������̽� �Լ� ȣ��.
				HitInterface->TraceHit_Implementation(WeaponTraceHit);

				// �ǰݵ� ���Ͱ� ���̶�� Damage�� ó��.
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
	// �Ѿ� Ÿ�Կ� ����, �Ѿ��� �ִ� �������� ���� �����Ǿ� �ִٸ�, �Ѿ��� ����.
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

// ���� �߿� ���¹̳ʸ� ����/ ���� ���°� �ƴϸ� ���¹̳ʸ� ����
void AFPShooterHero::CalculateStamina()
{	
	if (bIsSprint)
	{
		// ���� ���̶�� Stamina�� ����
		if (Stamina <= 0.0f)
		{
			//Stamina�� 0���� ���� ����.
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
		// ���� ���� �ƴ϶�� Stamina�� ����.
		if (Stamina >= 100.0f)
		{
			//Stamina�� 100�̻� ���� ����.
			Stamina = 100.0f;
		}
		else
		{
			Stamina += 0.25;
		}
	}

	// ���� ���¿� ���� ũ�ν��� ����ų� ǥ��.
	HiddenCrosshair(bIsSprint);

	/* DEBUG_LINE */
	if (GEngine)
	{
		FString StaminaAmount = FString::Printf(TEXT("Stamina is: %f"), Stamina);

		GEngine->AddOnScreenDebugMessage(0, -1, FColor::Yellow, StaminaAmount);
	}
}

// ���� ũ�ν������ Ȯ�� ���
void AFPShooterHero::CalcuateCrosshairSpreadMultiplier(float DeltaTime)
{
	// ���� ���ο� ���� ũ�ν���� Ȯ��
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

	// �ݹ� ���ο� ���� ũ�ν���� Ȯ��.
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

	// ADS ���ο� ���� ũ�ν���� Ȯ��.
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

// ũ�ν������ Ȯ�� ���� ������� ���Ѵ�.
float AFPShooterHero::GetCrosshairSpreadMultiplier()
{
	return CrosshairSpreadMultiplier;
}

// ũ�ν������ ǥ��/ ���� ó��.
void AFPShooterHero::HiddenCrosshair(bool bIsHidden)
{
	// ũ�ν��� ��ȿ�ϸ�
	if (HUDCrosshair)
	{
		if (bIsHidden)
		{
			// ũ�ν��� �����.
			HUDCrosshair->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			// ũ�ν��� ǥ���Ѵ�.
			HUDCrosshair->SetVisibility(ESlateVisibility::Visible);
		}

	}
}

// �Է¿� ���� ADS ���� ��ȯ�� ���� ī�޶� FOV�� ��ȭ ���� ó��.(�����Ӹ��� üũ)
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

// �Է¿� ���� Crouch ���� ��ȯ�� ��ġ ���� ó��. (�����Ӹ��� üũ)
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

// �������� ȹ��.
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
// ���� ����.

// �⺻ ���� ����.
AFPShooterWeapon* AFPShooterHero::SpawnDefaultWeapon()
{
	if (DefaultWeaponClass)
	{
		AFPShooterWeapon* DefaultWeapon;

		DefaultWeapon = GetWorld()->SpawnActor<AFPShooterWeapon>(DefaultWeaponClass);

		if (DefaultWeapon)
		{
			// �⺻ ���⸦ ù ��° ���Կ� ����
			WeaponSlot.Insert(DefaultWeapon, 0);
			return DefaultWeapon;
		}
	}
	return nullptr;
}

// ���⸦ ȹ��.
void AFPShooterHero::PickUpWeapon(AFPShooterWeapon* Weapon)
{
	if (Weapon == nullptr)
	{
		return;
	}

	if (HasAleadyPickUpWeapon(Weapon))
	{
		// ���Ⱑ ���� ���� �Ѿ� ������, ������ ���� ���� �Ѿ˿� ���ϰ�, ���͸� ����.
		StoreAmmo(Weapon->GetAmmoType(), Weapon->GetCurrentAmmoAmount());
		Weapon->Destroy();
		return;
	}

	// ���⸦ ������ ���Կ� �߰�.
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

// ���Կ� �̹� ���Ⱑ ��ϵǾ� �ִ� ���� �˻�.
bool AFPShooterHero::HasAleadyPickUpWeapon(AFPShooterWeapon* PickUpWeapon)
{
	// ���� ���Կ� ������ ���⸦ �̹� �����ϰ� �ִ����� üũ
	for (AFPShooterWeapon* Weapon : WeaponSlot)
	{
		if (Weapon)
		{
			/* DEBUG_LINE */
			UE_LOG(LogTemp, Warning, TEXT("Weapon Slot is: %d"), Weapon->GetWeaponSlotIndex());
			UE_LOG(LogTemp, Warning, TEXT("PickUpWeapon Slot is: %d"), PickUpWeapon->GetWeaponSlotIndex());

			// ���� �����Ұ� ����ִ� ���°� �ƴ϶��.
			if (Weapon->GetWeaponSlotIndex() == PickUpWeapon->GetWeaponSlotIndex())
			{
				/* DEBUG_LINE */
				UE_LOG(LogTemp, Warning, TEXT("Aleady Stored Weapon"));
				// ���� ���� ����� �Ⱦ��� ���Ⱑ �����ϴٸ�
				return true;
			}
		}
	}
	return false;
}

// ���⸦ ��ü
void AFPShooterHero::SwapWeapon(int32 WeaponIndex)
{
	// �Էµ� Index�� WeaponSlot�� ������ ����� �۵����� �ʴ´�.
	if (WeaponIndex < 0 || WeaponIndex >= WeaponSlot.Num())
	{
		return;
	}

	// �Էµ� Index�� �ش��ϴ� ���� ������ ���⸦ ���Ѵ�.
	AFPShooterWeapon* NewWeapon = WeaponSlot[WeaponIndex];

	// ���ο� ���Ⱑ ���� ������ ����� �����ϴٸ� �۵����� �ʴ´�.
	if (NewWeapon == EquippedWeapon)
	{
		return;
	}

	if (EquippedWeapon)
	{
		// ���� ���� ���Ⱑ ��ȿ�ϸ�
		// ���� ���� ���⸦ ������, ���� ����� ������ �ʵ��� ����.
		EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		EquippedWeapon->GetItemMesh()->SetVisibility(false);
	}

	EquipWeapon(NewWeapon);
}

// ���⸦ ����.
void AFPShooterHero::EquipWeapon(AFPShooterWeapon* Weapon)
{
	if (Weapon)
	{
		Weapon->GetItemCollision()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		Weapon->GetOverlapCollision()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

		FName GripSocketName;

		// Ÿ�Կ� ���� ������ �׸� ���Ͽ� ���Ⱑ �����ǵ��� ������ �̸��� ����.
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
		// ���Ӱ� ������ ���⸦ ���̵��� ����.
		EquippedWeapon->GetItemMesh()->SetVisibility(true);
	}
}

// �߻� Ÿ�̸� ����.
void AFPShooterHero::StartWeaponFire()
{
	/* DEBUG_LINE */
	UE_LOG(LogTemp, Warning, TEXT("Firing is %s"), bIsFiring ? TEXT("TRUE") : TEXT("FALSE"));

	if (EquippedWeapon == nullptr)
	{
		// ������ ���Ⱑ ��ȿ���� ������ �ƹ��� �۵��� ���� �ʴ´�.
		return;
	}

	if (EquippedWeapon->GetCurrentAmmoAmount() < 1)
	{
		// ������ ���⿡ �����ִ� �Ѿ��� ���ٸ� ������ Reload ����.
		bIsFiring = false;

		GetWorldTimerManager().ClearTimer(WeaponFireTimer);
		StartReload();
	}

	if (!bIsFiring && !bIsReload && EquippedWeapon->GetCurrentAmmoAmount() > 0)
	{
		// ���Ⱑ �߻� ���̶�� ���� ����.
		bIsFiring = true;

		// �Ѿ��� 1�� �Ҹ��Ѵ�.
		EquippedWeapon->ConsumeAmmo(1);

		// ������ �߻� Ÿ�Կ� ���� ������ �߻� ó�� �Լ��� ȣ��.
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

// ���� ������ �߻� Ÿ�̸� ����.
void AFPShooterHero::FinishFire()
{
	bIsFiring = false;

	GetWorldTimerManager().ClearTimer(WeaponFireTimer);

	/* DEBUG_LINE */
	UE_LOG(LogTemp, Warning, TEXT("Firing is End"));

	if (EquippedWeapon->GetCurrentAmmoAmount() < 1)
	{
		// ������ ���⿡ �����ִ� �Ѿ��� ���ٸ� ������ Reload ����.		
		StartReload();
	}
}

// �ڵ� ������ �߻� Ÿ�̸� ����.
void AFPShooterHero::FinishAutoFire()
{
	bIsFiring = false;

	if (EquippedWeapon->GetCurrentAmmoAmount() < 1)
	{
		// ������ ���⿡ �����ִ� �Ѿ��� ���ٸ� ������ Reload ����.
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

// �߻� ��Ÿ�� ���.
void AFPShooterHero::PlayFireMontage()
{
	// �߻� �ִϸ��̼� �ν��Ͻ� ȹ��.
	UAnimInstance* FireAnimInstance = FPShooterMesh->GetAnimInstance();

	// ��Ÿ�� ������ �̸��� ������ ���� ���� (�Ϲ�)
	FName FireMontageSectioName;

	// ��Ÿ�� ������ �̸��� ������ ���� ���� (ADS)
	FName ADSFireMontageSectionName;

	// ���� Ÿ�Կ� ����  �߻� ��Ÿ�� ���� �̸��� �����ϵ��� ����.
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

	// ���� ���� ���ΰ� ��ȿ�ϸ�.
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

// ���� �߻� ó��
void AFPShooterHero::WeaponFire()
{
	const FVector MuzzleSocketLocation = EquippedWeapon->GetMuzzleSocket()->GetSocketLocation(EquippedWeapon->GetItemMesh());
	FVector TraceLineEndPoint;

	// ��� ���� Ÿ�� ���θ� �˻�.
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
		// ��� ���� Ÿ�� ���θ� �˻�.
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

// ���� ���Կ��� ���� ����� ��ü
void AFPShooterHero::SwitchPrevWeapon()
{	
	// ���� ���� ���� �ε������� 1�� ����.
	int32 SlotNum = CurrentWeaponSlotIndex - 1;

	if (SlotNum < 0)
	{
		// ���� �ε����� 0���� �۰� �Ǹ�.
		// ���� ������ ������ ����� ��ü. (��ȯ ��ü)
		CurrentWeaponSlotIndex = WeaponSlot.Num() - 1;
	}
	else
	{
		if (WeaponSlot[SlotNum])
		{
			// �ش� ���Կ� ����� ���Ⱑ ��ȿ�ϴٸ�, 
			// �ּ� �ε������� �ִ� �ε��� ���̷� ���ѵ� ���� �ε����� ����.
			CurrentWeaponSlotIndex = FMath::Clamp(SlotNum, 0, WeaponSlot.Num() - 1);
		}
	}

	/* DEBUG_LINE */
	UE_LOG(LogTemp, Warning, TEXT("Current Weapon Slot is: %d"), CurrentWeaponSlotIndex);
}

// ���� ���Կ��� ���� ����� ��ü
void AFPShooterHero::SwitchNextWeapon()
{	
	// ������ ���� ���� �ε������� 1�� ����.
	int32 SlotNum = CurrentWeaponSlotIndex + 1;

	if (SlotNum > WeaponSlot.Num() - 1)
	{
		// ���� �ε�����, ���� ������ ũ�⺸�� ũ�� �Ǹ�.
		// �� ó�� ������ ����� ��ü. (��ȯ ��ü)
		CurrentWeaponSlotIndex = 0;
	}
	else
	{
		if (WeaponSlot[SlotNum])
		{
			// �ش� ���Կ� ����� ���Ⱑ ��ȿ�ϴٸ�, 
			// �ּ� �ε������� �ִ� �ε��� ���̷� ���ѵ� ���� �ε����� ����.
			CurrentWeaponSlotIndex = FMath::Clamp(SlotNum, 0, WeaponSlot.Num() - 1);
		}
	}
	
	/* DEBUG_LINE */
	UE_LOG(LogTemp, Warning, TEXT("Current Weapon Slot is: %d"), CurrentWeaponSlotIndex);
}

// ���Կ� ����� ���Ⱑ �ִٸ�, �ش� ���� Thumbnail�� Ȱ��ȭ�ϴ� �̺�Ʈ ȣ��.
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

// �Ⱦ��� ź���� ����.
void AFPShooterHero::PickUpAmmo(AFPShooterAmmo* Ammo)
{
	if (Ammo)
	{
		if (AmmoPack.Find(Ammo->GetAmmoType()))
		{
			// �Ⱦ��� ź��� ������ Ÿ���� ź���� ���� ���̶��, �Ⱦ��� ź��� ���� ���� ź���� �ջ��ؼ�, ���� ���� ź����
			// ������ �ݿ�.
			int32 AmmoAmount = Ammo->GetItemAmount() + AmmoPack[Ammo->GetAmmoType()];
			AmmoPack[Ammo->GetAmmoType()] = AmmoAmount;

			// ���͸� ����.
			Ammo->Destroy();
		}
	}
}

// ���� ������Ʈ
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
	// ��¡�� ���� ���ΰ� ��ȿ�ϸ�.
	if (bIsReload)
	{
		//������ �߿��� �� �ٽ� �������� ������� �ʵ��� ����.
		return;
	}

	// ���� ���� �Ѿ� �߿�, ������ ������ �Ѿ� Ÿ�԰� ������ Ÿ���� �Ѿ��� ������ ��ȿ�ϸ�.
	if (AmmoPack.Contains(EquippedWeapon->GetAmmoType()))
	{
		if (AmmoPack[EquippedWeapon->GetAmmoType()] <= 0)
		{
			/* DEBUG_LINE */
			UE_LOG(LogTemp, Warning, TEXT("No Ammo!!"));
			// ���� ���� �Ѿ��� ������, �ƹ��� ���۵� ���� �ʴ´�.
			return;
		}

		bIsReload = true;

		/* DEBUG_LINE */
		UE_LOG(LogTemp, Warning, TEXT("Reload Start"));

		// ������ �Ѿ��� ����
		int32 ReloadAmmoAmount = 0;

		// ���� ���� �Ѿ��� ������, ������ ������ ����ִ� źâ�� �������� ���� ���.
		if (AmmoPack[EquippedWeapon->GetAmmoType()] > EquippedWeapon->GetEmptyMagazineCapacity())
		{
			// ������ �Ѿ��� ������ ���Ѵ�.
			ReloadAmmoAmount = EquippedWeapon->GetEmptyMagazineCapacity();
			// ���� ��, ���� �Ѿ��� ������ ���Ѵ�.
			int32 RemainAmmoAmount = AmmoPack[EquippedWeapon->GetAmmoType()] - EquippedWeapon->GetEmptyMagazineCapacity();
			// ���� ���� �Ѿ��� ������ ���� ��, ���� �Ѿ��� ������ ����.
			AmmoPack.Add(EquippedWeapon->GetAmmoType(), RemainAmmoAmount);

			/*DEBUG_LINE*/
			UE_LOG(LogTemp, Warning, TEXT("A: Reload Ammo Amount is: %d"), ReloadAmmoAmount);
			UE_LOG(LogTemp, Warning, TEXT("A: Remain Ammo Amount is: %d"), RemainAmmoAmount);
		}

		// ���� ���� �Ѿ��� ������, ������ ������ ����ִ� źâ�� �������� ���� ���.
		if (AmmoPack[EquippedWeapon->GetAmmoType()] <= EquippedWeapon->GetEmptyMagazineCapacity())
		{
			// ������ �Ѿ��� ������ ���Ѵ�.
			ReloadAmmoAmount = AmmoPack[EquippedWeapon->GetAmmoType()];
			// ���� ��,���� �Ѿ��� ������ 0���� ����.
			int32 RemainAmmoAmount = 0;
			// ���� ���� �Ѿ��� ������ ���� ��, ���� �Ѿ��� ������ ����.
			AmmoPack.Add(EquippedWeapon->GetAmmoType(), RemainAmmoAmount);

			/*DEBUG_LINE*/
			UE_LOG(LogTemp, Warning, TEXT("B: Reload Ammo Amount is: %d"), ReloadAmmoAmount);
			UE_LOG(LogTemp, Warning, TEXT("B: Remain Ammo Amount is: %d"), RemainAmmoAmount);
		}

		EquippedWeapon->ChargeAmmo(ReloadAmmoAmount);

		// ������ �ִϸ��̼� ���.
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
// �÷��̾� �Է����� ���� ����.

// ������ ������/ �ڷ� �̵���Ű��.
void AFPShooterHero::HeroMoveForward(float AxisValue)
{
	if (Controller)
	{
		AddMovementInput(GetActorForwardVector(), AxisValue);
	}	
}

// ������ ��������/ ���������� �̵���Ű��.
void AFPShooterHero::HeroMoveRight(float AxisValue)
{
	if (Controller)
	{
		AddMovementInput(GetActorRightVector(), AxisValue);
	}	
}

// ������ �þ߸� ����/ �Ʒ��� ȸ����Ű��. (���콺 �Է�)
void AFPShooterHero::HeroLookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

// ������ �þ߸� ����/ �Ʒ��� ȸ����Ű��. (�����е� �Է�)
void AFPShooterHero::HeroLookUpGamepad(float AxisValue)
{
	AddControllerPitchInput(AxisValue * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

// ������ �þ߸� ��������/ ���������� ȸ����Ű��. (���콺 �Է�)
void AFPShooterHero::HeroTurn(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

// ������ �þ߸� ��������/ ���������� ȸ����Ű��. (�����е� �Է�)
void AFPShooterHero::HeroTurnGamepad(float AxisValue)
{
	AddControllerYawInput(AxisValue * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFPShooterHero::JumpPressed()
{
	// Crouch ���°� ��ȿ�ϸ�
	if (GetCharacterMovement()->IsCrouching())
	{
		// Crouch ���� ����.
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

// ������ �����ϱ�.
void AFPShooterHero::SpirntPressed()
{
	if (GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0f)
	{
		// �̵� ���� �ƴϸ� ������ �� ����.
		if (bIsReload || GetCharacterMovement()->IsFalling())
		{
			//Reload �����̰ų�, ���� ���̶�� ���� �Ұ�.
			return;
		}

		if (bIsADS)
		{
			// ADS ���¶��, ADS ���� ����.
			bIsADS = false;
		}

		if (GetCharacterMovement()->IsCrouching())
		{
			UnCrouch();
		}

		// ���� ���¸� true�� �����ϰ�, ������ �̵� �ӵ��� 2�� ����.
		bIsSprint = true;
		GetCharacterMovement()->MaxWalkSpeed = HeroWalkSpeed * 2.0f;
	}	
}

// ������ ���� ���߱�.
void AFPShooterHero::SprintReleased()
{
	if (bIsSprint)
	{
		bIsSprint = false;
		GetCharacterMovement()->MaxWalkSpeed = HeroWalkSpeed;
	}	
}

// Ʈ���� ��ư�� �Է��Ͽ� �߻��ϱ�.
void AFPShooterHero::TriggerPressed()
{
	if (bIsReload)
	{
		// Reload ���¿����� �ݹ� �Ұ�.
		return;
	}

	// Ʈ���� �Է� ��ȿ.
	bIsPullTheTrigger = true;

	// ���� �߻縦 ����.
	StartWeaponFire();	
}

// Ʈ���� ��ư�� �Է��� �����Ͽ� �߻� ���߱�.
void AFPShooterHero::TriggerReleased()
{
	bIsPullTheTrigger = false;
}

// ADS ��� �Է�.
void AFPShooterHero::ADSPressed()
{
	if (bIsReload || bIsSprint)
	{
		// Reload �Ǵ� Sprint ���¿����� ADS ���� �Ұ�.
		return;
	}

	// ������ ADS ��� ���� ���ο� ���� ADS ����.
	if (!EquippedWeapon->GetPossibleADS())
	{
		return;
	}
	
	bIsADS = true;
	GetCharacterMovement()->MaxWalkSpeed = HeroWalkSpeed * 0.75;		
}

// ADS ��� ����
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
	// ���� ���̸� Crouch �Ұ�.
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	// Crouch ���°� ��ȿ�ϸ�.
	if (GetCharacterMovement()->IsCrouching())
	{
		// Crouch ���¸� �����ϰ�, ī�޶��� ���̸� ����.	
		UnCrouch();
	}
	else
	{	
		// Crouch ���·� ��ȯ�ϰ�, ī�޶��� ���̸� Crouch ���·� ����.
		Crouch();
	}	
}

void AFPShooterHero::ReloadPressed()
{
	if (bIsSprint || bIsPullTheTrigger)
	{
		// Sprint �Ǵ� �ݹ� ���¶�� Reload ���� �Ұ�.
		return;
	}

	if (bIsADS)
	{
		// ADS ���¶��, ADS ���� ����.
		bIsADS = false;
	}
	
	//Reload�� �����Ѵ�.
	StartReload();
}

// ���� ������ ������ ���� ���Կ� ��ϵ� ����� ��ü.
void AFPShooterHero::SwitchPrevWeaponPressed()
{
	// ���� ������ ����� ��ü�ϰ�, ��ü�� ���� ���� ������� ������ ����.
	SwitchPrevWeapon();
	SwapWeapon(CurrentWeaponSlotIndex);
	EquipWeapon(WeaponSlot[CurrentWeaponSlotIndex]);
	EquippedWeaponSlot(EquippedWeapon->GetWeaponSlotIndex());	
}

// ���� ������ ������ ���� ���Կ� ��ϵ� ����� ��ü.
void AFPShooterHero::SwitchNextWeaponPressed()
{
	// ���� ������ ����� ��ü�ϰ�, ��ü�� ���� ���� ������� ������ ����.
	SwitchNextWeapon();
	SwapWeapon(CurrentWeaponSlotIndex);
	EquipWeapon(WeaponSlot[CurrentWeaponSlotIndex]);
	EquippedWeaponSlot(EquippedWeapon->GetWeaponSlotIndex());
}

void AFPShooterHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ��(Axis) �Է� �Լ� ���ε�
	PlayerInputComponent->BindAxis(FName("Move Forward / Backward"), this, &AFPShooterHero::HeroMoveForward);
	PlayerInputComponent->BindAxis(FName("Move Right / Left"), this, &AFPShooterHero::HeroMoveRight);
	PlayerInputComponent->BindAxis(FName("Look Up / Down Mouse"), this, &AFPShooterHero::HeroLookUp);
	PlayerInputComponent->BindAxis(FName("Look Up / Down Gamepad"), this, &AFPShooterHero::HeroLookUpGamepad);
	PlayerInputComponent->BindAxis(FName("Turn Right / Left Mouse"), this, &AFPShooterHero::HeroTurn);
	PlayerInputComponent->BindAxis(FName("Turn Right / Left Gamepad"), this, &AFPShooterHero::HeroTurnGamepad);

	// �׼�(Button) �Է� �Լ� ���ε�
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
