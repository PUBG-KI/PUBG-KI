// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Movement/PlayerMovementComponent.h"
#include "Character/PlayerCharacter.h"

float UPlayerMovementComponent::GetMaxSpeed() const
{
	APlayerCharacter* Owner = Cast<APlayerCharacter>(GetOwner());
	float BaseSpeed = Owner->GetMoveSpeed();

	// if(Owner->IsSprint)
	// 	sprint = 2.0f;
	// if(Owner->IsBack)
	// {
	// 	return 100.f;
	// }
	
	if (Owner->bIsProne)
	{
		if (Owner->bIsWalk)
		{
			return BaseSpeed - SubCTRLProne - SubProne; // PRONE+CTRL : 350 - 60 - 240 = 50; 
		}
		return BaseSpeed - SubProne; // PRONE : 350-240 = 110;
	}
	if (Owner->bIsCrouch)
	{
		if (Owner->bIsSprint)
		{
			if (Owner->MoveForwardVecter.Y > 0)
			{
				return BaseSpeed + AddSHIFTSprint - Subcrouch; //Crouch+SHIFT : 350 + 150 - 100 = 400;
			}
		}
		if (Owner->bIsWalk)
		{
			return BaseSpeed - SubCTRLCrouch -Subcrouch; // Crouch+ctrl : 350 - 70 - 100 = 80;
		}
		return BaseSpeed - Subcrouch; // crouch : 350-100 = 250;
	}

	if (Owner->bIsSprint)
	{
		if (Owner->MoveForwardVecter.Y > 0)
		{
			return BaseSpeed + AddSHIFTSprint; //STAND+SHIFT : 350+150 = 500;
		}
	}
	if (Owner->bIsWalk)
	{
		return BaseSpeed - SubCTRLWalk; //STAND-CTRL : 350 - 200 = 150;
	}
	
	return BaseSpeed; //350
}
// 걷기 350 
// 수그리기 250
// 눕기 110
//
// 쉬프트+걷기 500 
// 쉬프트+수그리기 400 
//
// 컨트롤+걷기 150;
// 컨트롤+수그리기 80
// 컨트롤+눕기 50