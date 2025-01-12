// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayTagContainer.h"
#include "BaseAnimInstance.generated.h"

class ABaseCharacter;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class PUBG_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override; 
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds);

	UPROPERTY()
	ABaseCharacter* OwningCharacter;	
	UPROPERTY()
	UCharacterMovementComponent* OwningMovementComponent;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Locomotion")
	float GroundSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Locomotion")
	bool bHasAcceleration;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Locomotion")
	bool bIsRun;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Locomotion")
	bool bIsFalling;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Locomotion")
	bool bIsCrouching;
	
protected:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	bool OwnerHasTag(FGameplayTag Tag) const;
};
