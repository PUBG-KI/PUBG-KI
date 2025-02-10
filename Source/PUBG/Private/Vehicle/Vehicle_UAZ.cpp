// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle/Vehicle_UAZ.h"
#include "Character/PlayerCharacter.h"
#include "AnimInstance/PlayerAnimInstance.h"
void AVehicle_UAZ::InteractWith_Implementation(APlayerCharacter* Character)
{
	IInteractInterface::InteractWith_Implementation(Character);
Character->OntheVehicle = true;
}

FText AVehicle_UAZ::LookAt()
{
	return Super::LookAt();
}
