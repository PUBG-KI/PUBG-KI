#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ArmorStruct.generated.h"

USTRUCT(BlueprintType)
struct FArmorStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* StaticMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* SkeletalMesh;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Durability; // 내구도

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Defense; // 방어력

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Image;
};