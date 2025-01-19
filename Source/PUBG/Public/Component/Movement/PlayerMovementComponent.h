// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class PUBG_API UPlayerMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	
	virtual float GetMaxSpeed() const override;
	float AddSHIFTSprint = 150.0f;
	float SubProne = 240.0f;
	float Subcrouch = 100.f;
	float SubCTRLWalk = 200.f;
	float SubCTRLProne = 60.f;
	float SubCTRLCrouch =170.f;
	
};
