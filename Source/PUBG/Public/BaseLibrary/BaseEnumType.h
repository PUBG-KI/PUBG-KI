#pragma once

UENUM()
enum class EBaseConfirmType : uint8
{
	Yes,
	No,
};

UENUM()
enum class EBaseValidType : uint8
{
	Valid,
	InValid,
};

UENUM()
enum class EBaseSuccessType : uint8
{
	Success,
	Failed,
};

UENUM(BlueprintType)
enum class EItemZoneType : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	NoneArea UMETA(DisplayName = "NoneArea"),
	NearArea UMETA(DisplayName = "NearArea"),
	InventoryArea UMETA(DisplayName = "InventoryArea"),
	EquippedArea UMETA(DisplayName = "EquippedArea"),
};
