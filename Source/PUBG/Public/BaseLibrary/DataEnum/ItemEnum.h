#pragma once
#include "CoreMinimal.h"
#include "ItemEnum.generated.h"

UENUM(BlueprintType)
enum class EItemCategory : uint8
{
	MainWeapon = 0 UMETA(Display = "MainWeapon"),
	SubWeapon  = 2 UMETA(Display = "SubWeapon"),
	MeleeWeapon UMETA(Display = "MeleeWeapon"),
	Throw UMETA(Display = "Throw"),
	
	Helmet UMETA(Display = "Helmet"),
	Bag UMETA(Display = "Bag"),
	Vest UMETA(Display = "Vest"),
	Belt UMETA(Display = "Belt"),
	
	Ammo UMETA(Display = "Ammo"),
	
	Heal UMETA(Display = "Heal"),
	Booster UMETA(Display = "Booster"),
	
	WeaponPart UMETA(Display = "WeaponPart"),

	Head = 9 UMETA(Display = "Head"),
	Mask UMETA(Display = "Mask"),
	Glasses UMETA(Display = "Glasses"),

	Shirt UMETA(Display = "Shirts"),
	Outerwear UMETA(Display = "Outerwear"),
	Glove UMETA(Display = "Glove"),
	FullBody UMETA(Display = "FullBody"),

	Pants UMETA(Display = "Pants"),
	Shoes UMETA(Display = "Shoes"),
};

UENUM(BlueprintType)
enum class EEquippedItemCategory : uint8
{
	PrimarySlot = 0 UMETA(Display = "PrimarySlot"),
	SecondarySlot UMETA(Display = "SecondarySlot"),
	SubWeapon UMETA(Display = "SubWeapon"),
	MeleeWeapon UMETA(Display = "MellWeapon"),
	Throw UMETA(Display = "Throw"),
	
	
	
};
