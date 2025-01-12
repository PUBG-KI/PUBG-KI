// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/BaseAnimInstance.h"
#include "Character/BaseCharacter.h"
#include "BaseLibrary/BaseFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBaseAnimInstance::NativeInitializeAnimation()
{
	OwningCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
	if (OwningCharacter)
	{
		OwningMovementComponent = OwningCharacter->GetCharacterMovement();
	}
}

void UBaseAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (!OwningCharacter || !OwningMovementComponent)
	{
		return;
	}

	GroundSpeed = OwningCharacter->GetVelocity().Size2D();

	bHasAcceleration = OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f;

	bIsRun = OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 250.f;

	bIsFalling = OwningMovementComponent->IsFalling();
	bIsCrouching = OwningMovementComponent->IsCrouching();
}

bool UBaseAnimInstance::OwnerHasTag(FGameplayTag Tag) const
{
	if (APawn* OwningPawn = TryGetPawnOwner())
	{
		return UBaseFunctionLibrary::NativeActorHasTag(OwningPawn, Tag);
	}
	return false;
}
