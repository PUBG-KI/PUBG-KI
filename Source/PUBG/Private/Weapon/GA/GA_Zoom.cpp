// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GA/GA_Zoom.h"

#include "Camera/CameraComponent.h"
#include "Character/PlayerCharacter.h"
#include "Component/Equipped/EquippedComponent.h"
#include "Component/Movement/PlayerMovementComponent.h"

void UGA_Zoom::ActivatedZoom(USkeletalMeshComponent* GunMesh, UCameraComponent* ActivateCamera,
                             UCameraComponent* DeActivateCamera)
{
	if (GetPlayerCharacterFromActorInfo()->GetEquippedComponent()->GetCurrentWeapon())
	{
		UPlayerMovementComponent* Movementcomponent = Cast<UPlayerMovementComponent>(GetPlayerCharacterFromActorInfo()->GetMovementComponent());
		
		ActivateCamera->AttachToComponent(GunMesh, FAttachmentTransformRules::KeepRelativeTransform, "AimSocket");
		DeActivateCamera->SetActive(false);
		ActivateCamera->SetActive(true);
		
		ActivateCamera->bUsePawnControlRotation = false;
		//Movementcomponent->bUseControllerDesiredRotation = true;

		GetPlayerCharacterFromActorInfo()->IsZoom = true;
		GetPlayerCharacterFromActorInfo()->SetcurrentCamera(ActivateCamera);
		ActivateCamera->SetFieldOfView(75.0f);
	}
}

void UGA_Zoom::DeActivatedZoom(UCameraComponent* ActivateCamera, UCameraComponent* DeActivateCamera)
{
	if (GetPlayerCharacterFromActorInfo()->GetEquippedComponent()->GetCurrentWeapon())
	{
		UPlayerMovementComponent* Movementcomponent = Cast<UPlayerMovementComponent>(GetPlayerCharacterFromActorInfo()->GetMovementComponent());

		DeActivateCamera->AttachToComponent(GetPlayerCharacterFromActorInfo()->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "camera_fppSocket");
		DeActivateCamera->SetActive(false);
		ActivateCamera->SetActive(true);

		Movementcomponent->bUseControllerDesiredRotation = false;
		
		GetPlayerCharacterFromActorInfo()->IsZoom = false;
		GetPlayerCharacterFromActorInfo()->SetcurrentCamera(ActivateCamera);
	}
}
