// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Movement/PlayerMovementComponent.h"
#include "Character/PlayerCharacter.h"

float UPlayerMovementComponent::GetMaxSpeed() const
{
	APlayerCharacter* Owner = Cast<APlayerCharacter>(GetOwner());

	float sprint = 1.0f;
	// if(Owner->IsSprint)
	// 	sprint = 2.0f;
	 // if(Owner->IsBack)
	 // {
	 // 	return 100.f;
	 // }
	if(Owner->bIsProne)
	{
		return (Owner->GetMoveSpeed() - 3.f) * sprint;
	}
	else if(Owner->bIsCrouched)
	{
		return Owner->GetMoveSpeed() * 0.5f * sprint;		
	}
	
	return Owner->GetMoveSpeed() * sprint;
}
