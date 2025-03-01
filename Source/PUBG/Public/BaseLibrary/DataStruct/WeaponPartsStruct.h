#pragma once
#include "ItemStruct.h"

#include "WeaponPartsStruct.generated.h"


USTRUCT(BlueprintType)
struct FWeaponPartsStruct : public FTableRowBase
{
	GENERATED_BODY()

	FWeaponPartsStruct()
	{
		PartsName = NAME_None;
		Weight = -1.0f;
		ItemCategory = EItemCategory::FullBody;
		StaticMesh = nullptr;
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName PartsName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemCategory ItemCategory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* StaticMesh;
};
