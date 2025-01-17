#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"

#include "ItemStruct.generated.h"


enum class ItemEnum : uint8;

USTRUCT(BlueprintType)
struct FItemStruct : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Description;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh* StaticMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* Image;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Weight;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ItemEnum Category;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsStackAble;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 StackSize;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Quantity;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag Tag;
	
	
};
