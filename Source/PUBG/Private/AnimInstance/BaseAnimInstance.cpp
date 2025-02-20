// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/BaseAnimInstance.h"

#include "KismetAnimationLibrary.h"

#include "BaseLibrary/BaseDebugHelper.h"

#include "Kismet/KismetMathLibrary.h"
#include "VectorTypes.h"

#include "Character/BaseCharacter.h"
#include "BaseLibrary/BaseFunctionLibrary.h"
#include "Character/PlayerCharacter.h"
#include "Component/Movement/PlayerMovementComponent.h"
#include "Net/UnrealNetwork.h"


void UBaseAnimInstance::NativeInitializeAnimation()
{
	OwningCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
	if (OwningCharacter)
	{
		OwningMovementComponent = Cast<UPlayerMovementComponent>(OwningCharacter->GetCharacterMovement());
		OwningPlayer = Cast<APlayerCharacter>(OwningCharacter);
	}
}

void UBaseAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (!OwningCharacter || !OwningMovementComponent)
	{
		return;
	}
	

	Velocity = OwningCharacter->GetVelocity();
	GroundSpeed = Velocity.Size2D();
	Direction = UKismetAnimationLibrary::CalculateDirection(OwningCharacter->GetVelocity(),
	                                                        OwningCharacter->GetActorRotation());

	bHasAcceleration = OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f;
	bHasAcceleration = GroundSpeed > 0.f;

	bIsRun = GroundSpeed > 350.f;

	bIsFalling = OwningMovementComponent->IsFalling();
	bIsCrouching = OwningMovementComponent->IsCrouching();
	bIsProne = OwningMovementComponent->RequestToStartProne;

	//bIsProne = OwningPlayer->GetIsProne();
	if (bIsFalling)
	{
		FallingTime += DeltaSeconds;
	}
	else
	{
		FallingTime = 0.f;
	}
	//턴과 에임오프셋을 위한 Yaw,Pitch구하기

	FRotator AimRotation = OwningCharacter->GetBaseAimRotation();
	 FRotator ActorRotation = OwningPlayer->GetActorRotation();
	// UE_LOG(LogTemp, Log, TEXT("AimRotation: %s, ActorRotation: %s"), *AimRotation.ToString(),
	//        *ActorRotation.ToString());
	 FRotator DeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, ActorRotation);
	//if (OwningCharacter->HasAuthority() == false)
	Yaw = OwningPlayer->Yaw;
	Pitch = DeltaRotator.Pitch;
	//UE_LOG(LogTemp, Log, TEXT("Pitch: %f, Yaw: %f"), Pitch, Yaw);
	LeaningPressedValue = OwningMovementComponent->LeaningValue;
	PlayerOntheVehicle = OwningPlayer->GetOnTheVehicle();
	VehicleFacetoBackward = OwningPlayer->GetVehicleFacetoBackward();
	VehicleVelocityBackWard = OwningPlayer->GetVehicleVelocityBackWard();
}

bool UBaseAnimInstance::OwnerHasTag(FGameplayTag Tag) const
{
	if (APawn* OwningPawn = TryGetPawnOwner())
	{
		return UBaseFunctionLibrary::NativeActorHasTag(OwningPawn, Tag);
	}
	return false;
}

void UBaseAnimInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBaseAnimInstance, Yaw);
	DOREPLIFETIME(UBaseAnimInstance, Pitch);
}

// void UBaseAnimInstance::MulticastSetAimRotation_Implementation(FRotator NewAimRotation)
// {
// 	AimRotation = NewAimRotation;
// }
//
// void UBaseAnimInstance::ServerSetAimRotation_Implementation(FRotator NewAimRotation)
// {
// 	MulticastSetAimRotation(NewAimRotation);
// }
