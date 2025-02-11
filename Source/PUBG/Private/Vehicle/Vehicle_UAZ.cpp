// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle/Vehicle_UAZ.h"
#include "Character/PlayerCharacter.h"
#include "AnimInstance/PlayerAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Component/PUBGSpringArmComponent.h"
#include "Components/ArrowComponent.h"
#include "EnhancedInputSubsystems.h"
void AVehicle_UAZ::InteractWith_Implementation(APlayerCharacter* Character)
{
	IInteractInterface::InteractWith_Implementation(Character);
	TSubclassOf<UPlayerAnimInstance> UAZAnimLayer = LoadClass<UPlayerAnimInstance>(nullptr, TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprint/Character/Animation/AnimLayer/Character_VehicleAnimLayer/ABP_UAZ.ABP_UAZ'"));
	Character->Server_SetAnimLayer(UAZAnimLayer);
	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	if (Character->GetOnTheVehicle())
	{
		UE_LOG(LogTemp, Warning, TEXT("sadf123"));
		Character->SetOnTheVehicle(false);
		PlayerController->UnPossess();
		PlayerController->Possess(Character);
		Character->SetActorEnableCollision(true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("sadf"));
		Character->SetOnTheVehicle(true); //애님인스턴스로 보내기 위함
		Character->SetActorEnableCollision(false); // 콜리전을 꺼서 차량안으로 들어갈 수 있도록
		Character->AttachToComponent(ArrowComponent,FAttachmentTransformRules::SnapToTargetNotIncludingScale);

		FRotator CharacterRotator = Character->GetCameraBoom()->GetTargetRotation();
		
		PlayerController->UnPossess(); // 기존 포세스를 해제
		PlayerController->Possess(this);
		PlayerController->SetControlRotation(CharacterRotator);
		
		ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
		check(Subsystem);
		//Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);
	}
}

FText AVehicle_UAZ::LookAt()
{
	return Super::LookAt();
}
