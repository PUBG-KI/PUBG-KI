// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Movement/PlayerMovementComponent.h"
#include "Character/PlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Net/UnrealNetwork.h"

UPlayerMovementComponent::UPlayerMovementComponent()
{
	SetIsReplicatedByDefault(true); 
}

void UPlayerMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UPlayerMovementComponent, RequestToStartProne);
}

float UPlayerMovementComponent::GetMaxSpeed() const
{                                                                                                                             
	APlayerCharacter* Owner = Cast<APlayerCharacter>(GetOwner());
	float BaseSpeed = Owner->GetMoveSpeed();

	// if(Owner->IsSprint)
	// 	sprint = 2.0f;
	

	// 뒤로 가기 할 때 는
	// if(Owner->IsBack)
	// {
	//     BaseSpeed -= 100.f; // 이런식으로 빼면 됨
	// }
	
	if (RequestToStartProne)
	{
		BaseSpeed -= SubProne; // PRONE : 350-240 = 110;
	}
	else if (IsCrouching())
	{
		BaseSpeed -= Subcrouch; // crouch : 350-100 = 250;
	}

	if (RequestToStartSprinting)
	{
		BaseSpeed += AddSHIFTSprint; //STAND+SHIFT : 350+150 = 500;
	}
	else if (RequestToStartWalking)
	{
		BaseSpeed -= SubCTRLWalk; //STAND-CTRL : 350 - 200 = 150;
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


void UPlayerMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);
	
	//Flags 매개변수에는 저장된 이동에 저장된 압축된 입력 플래그가 포함되어 있습니다.
	//UpdateFromCompressed 플래그는 단순히 저장된 이동의 플래그를 이동 구성요소에 복사합니다.
	//기본적으로 이동 구성요소를 이동이 이루어졌을 때의 상태로 재설정하므로 거기에서 시뮬레이션할 수 있습니다.
	RequestToStartSprinting = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
	RequestToStartWalking = (Flags & FSavedMove_Character::FLAG_Custom_1) != 0;
	RequestToStartProne = (Flags & FSavedMove_Character::FLAG_Custom_2) != 0;	
}

void UPlayerMovementComponent::FGDSavedMove::Clear()
{
	Super::Clear();

	SavedRequestToStartSprinting = false;
	SavedRequestToStartWalking = false;
	SavedRequestToStartProne = false;
}

uint8 UPlayerMovementComponent::FGDSavedMove::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (SavedRequestToStartSprinting)
	{
		Result |= FLAG_Custom_0;
	}

	if (SavedRequestToStartWalking)
	{
		Result |= FLAG_Custom_1;
	}
	
	if (SavedRequestToStartProne)
	{
		Result |= FLAG_Custom_2;
	}

	return Result;
}

bool UPlayerMovementComponent::FGDSavedMove::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character,
	float MaxDelta) const
{
	//Set which moves can be combined together. This will depend on the bit flags that are used.
	if (SavedRequestToStartSprinting != ((FGDSavedMove*)&NewMove)->SavedRequestToStartSprinting)
	{
		return false;
	}

	if (SavedRequestToStartWalking != ((FGDSavedMove*)&NewMove)->SavedRequestToStartWalking)
	{
		return false;
	}
	
	if (SavedRequestToStartProne != ((FGDSavedMove*)&NewMove)->SavedRequestToStartProne)
	{
		return false;
	}

	return Super::CanCombineWith(NewMove, Character, MaxDelta);
}

void UPlayerMovementComponent::FGDSavedMove::SetMoveFor(ACharacter* Character, float InDeltaTime,
	FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	UPlayerMovementComponent* CharacterMovement = Cast<UPlayerMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
		SavedRequestToStartSprinting = CharacterMovement->RequestToStartSprinting;
		SavedRequestToStartWalking = CharacterMovement->RequestToStartWalking;
		SavedRequestToStartProne = CharacterMovement->RequestToStartProne;		
	}
}

void UPlayerMovementComponent::FGDSavedMove::PrepMoveFor(class ACharacter* Character)
{
	Super::PrepMoveFor(Character);

	UPlayerMovementComponent* CharacterMovement = Cast<UPlayerMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
	}
}


UPlayerMovementComponent::FGDNetworkPredictionData_Client::FGDNetworkPredictionData_Client(const UCharacterMovementComponent & ClientMovement)
	: Super(ClientMovement)
{
	
}

FSavedMovePtr UPlayerMovementComponent::FGDNetworkPredictionData_Client::AllocateNewMove()
{
	return FSavedMovePtr(new FGDSavedMove());
}

class FNetworkPredictionData_Client* UPlayerMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != NULL);

	if (!ClientPredictionData)
	{
		UPlayerMovementComponent* MutableThis = const_cast<UPlayerMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FGDNetworkPredictionData_Client(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}

void UPlayerMovementComponent::StartSprinting()
{
	if (!RequestToStartProne)
	{
		RequestToStartSprinting = true;
		RequestToStartWalking = false;
	}
}

void UPlayerMovementComponent::StopSprinting()
{
	RequestToStartSprinting = false;
}

void UPlayerMovementComponent::StartWalking()
{
	RequestToStartWalking = true;
	RequestToStartSprinting = false;
}

void UPlayerMovementComponent::StopWalking()
{
	RequestToStartWalking = false;		
}

void UPlayerMovementComponent::StartProne()
{
	RequestToStartProne = true;
}

void UPlayerMovementComponent::StopProne()
{
	RequestToStartProne = false;
}
