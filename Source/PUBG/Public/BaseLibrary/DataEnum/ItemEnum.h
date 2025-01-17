#pragma once
#include "CoreMinimal.h"
#include "ItemEnum.generated.h"

UENUM(BlueprintType)
enum class ItemEnum : uint8
{
	Weapon = 0 UMETA(Display = "Weapon"),
	Armor UMETA(Display = "Armor"),
	Ammo UMETA(Display = "Ammo"),
	Heal UMETA(Display = "Heal"),
	WeaponPart UMETA(Display = "WeaponPart"),
	Throw UMETA(Display = "Throw"),
};
