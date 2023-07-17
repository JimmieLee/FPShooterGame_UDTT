#pragma once

// 무기의 타입
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    EWT_Pistol UMETA(DisplayName = "Pistol"),
    EWT_AssultRifle UMETA(DisplayName = "Assult Rifle"),
    EWT_Shotgun UMETA(DisplayName = "Shotgun"),
    EWT_GrenadeLauncher UMETA(DisplayName = "Grenade Launcher"),
    EWT_RocketLauncher UMETA(DisplayName = "Rocket Launcher"),
    EWT_SniperRifle UMETA(DisplayName = "Sniper Rifle"),

	EWT_MAX UMETA(DisplayName = "Default Weapon")
};

//총알의 타입
UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	EAT_Pistol UMETA(DisplayName = "9mm"),
	EAT_AR UMETA(DisplayName = "5.56mm"),
	EAT_Shotgun UMETA(DisplayName = "Shell"),
	EAT_GL UMETA(DisplayName = "Grenade"),
	EAT_RL UMETA(DisplayName = "Rockeet"),
	EAT_SR UMETA(DisplayName = "7,62mm"),

	EAT_MAX UMETA(DisplayName = "Default Ammo")
};