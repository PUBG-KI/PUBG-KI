// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/GA/GA_Zoom.h"

#include "Camera/CameraComponent.h"
#include "Character/PlayerCharacter.h"
#include "Component/Equipped/EquippedComponent.h"
#include "Component/Movement/PlayerMovementComponent.h"

void UGA_Zoom::ActivatedZoom(USkeletalMeshComponent* GunMesh, UCameraComponent* ActivateCamera,
                             UCameraComponent* DeActivateCamera)
{
	APlayerCharacter* playercharacter = GetPlayerCharacterFromActorInfo();
	
	if (playercharacter->GetEquippedComponent()->GetCurrentWeapon())
	{
		UPlayerMovementComponent* Movementcomponent = Cast<UPlayerMovementComponent>(GetPlayerCharacterFromActorInfo()->GetMovementComponent());
		
		FString ScopeName = playercharacter->GetEquippedComponent()->GetCurrentWeapon_GunBase()->GetWeaponParts()[1].PartsName.ToString();
		float FOV_Value = playercharacter->GetEquippedComponent()->GetCurrentWeapon_GunBase()->GetWeaponParts()[1].ChangeValue;
		
		FString GunName = playercharacter->GetEquippedComponent()->GetCurrentWeapon()->GetWeaponDataAsset().GunName;
		
		ScopeName.ReplaceInline(*FString(TEXT("_") + GunName), TEXT(""));
		FName NewPartsName = FName(*ScopeName);

		if (NewPartsName == NAME_None)
		{
			ActivateCamera->AttachToComponent(GunMesh, FAttachmentTransformRules::KeepRelativeTransform, "AimSocket_IronSight");
			ActivateCamera->SetFieldOfView(90.0f);
		}
		else if (NewPartsName == "DotSight")
		{
			ActivateCamera->AttachToComponent(GunMesh, FAttachmentTransformRules::KeepRelativeTransform, "AimSocket_RDS");
			ActivateCamera->SetFieldOfView(FOV_Value);
		}
		else if (NewPartsName == "ACOG_01")
		{
			ActivateCamera->AttachToComponent(GunMesh, FAttachmentTransformRules::KeepRelativeTransform, "AimSocket_ACOG");
			ActivateCamera->SetFieldOfView(FOV_Value);
		}
		else if (NewPartsName == "CQBSS")
		{
			ActivateCamera->AttachToComponent(GunMesh, FAttachmentTransformRules::KeepRelativeTransform, "AimSocket_CQBSS");
			ActivateCamera->SetFieldOfView(FOV_Value);
		}
		
		DeActivateCamera->SetActive(false);
		ActivateCamera->SetActive(true);
		
		//ActivateCamera->bUsePawnControlRotation = false;
		Movementcomponent->bUseControllerDesiredRotation = true;

		GetPlayerCharacterFromActorInfo()->IsZoom = true;
		GetPlayerCharacterFromActorInfo()->SetCurrentCamera(ActivateCamera);

	}
}

void UGA_Zoom::DeActivatedZoom(UCameraComponent* ActivateCamera, UCameraComponent* DeActivateCamera)
{
	if (GetPlayerCharacterFromActorInfo()->GetEquippedComponent()->GetCurrentWeapon())
	{
		UPlayerMovementComponent* Movementcomponent = Cast<UPlayerMovementComponent>(GetPlayerCharacterFromActorInfo()->GetMovementComponent());
		
		DeActivateCamera->SetActive(false);
		ActivateCamera->SetActive(true);
		DeActivateCamera->AttachToComponent(GetPlayerCharacterFromActorInfo()->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "camera_fppSocket");
		DeActivateCamera->SetFieldOfView(90.0f);


		Movementcomponent->bUseControllerDesiredRotation = false;
		
		GetPlayerCharacterFromActorInfo()->IsZoom = false;
		GetPlayerCharacterFromActorInfo()->SetCurrentCamera(ActivateCamera);
	}
}
