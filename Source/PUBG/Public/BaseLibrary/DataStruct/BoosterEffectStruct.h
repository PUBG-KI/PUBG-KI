#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BoosterEffectStruct.generated.h"

USTRUCT(BlueprintType)
struct FBoosterEffectStruct : public FTableRowBase
{
	GENERATED_BODY()

	FBoosterEffectStruct()
	{
		Name = NAME_None;
		SkeletalMesh = nullptr;
		StaticMesh = nullptr;
		AnimationAsset = nullptr;
		UseTime = -1.0f;
		StaminaChargeValue = -1.0f;
		AnimationMontage = nullptr;
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* SkeletalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* AnimationAsset; // 아이템 사용 시 아이템 애니메이션 에셋 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float UseTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StaminaChargeValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AnimationMontage; // 아이템 사용 시 캐릭터 애니메이션 몽타주 
};

