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
};
