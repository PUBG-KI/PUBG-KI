// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Player/GA_Shouldering.h"
#include "Component/PUBGSpringArmComponent.h"
#include "Character/PlayerCharacter.h"


UGA_Shouldering::UGA_Shouldering()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
}

void UGA_Shouldering::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{UE_LOG(LogTemp, Warning, TEXT("sdaf"));
	PlayerCameraBoom = GetPlayerCharacterFromActorInfo()->GetCameraBoom();
	if (GetPlayerCharacterFromActorInfo()->GetCameraMode() == PlayerCameraMode::FPPCamera)
	{PlayerCameraBoom->SetWanstReversePlaying(false);
		FVector OffsetDelta = FVector(110.f, 10.f, 0.f);
		float Duration = 0.2f;
		PlayerCameraBoom->TimelineAddOffset(OffsetDelta, Duration);
	}
	// PlayerCameraBoom = GetPlayerCharacterFromActorInfo()->GetCameraBoom();
	// if (PlayerCameraBoom&&bIsOnce)
	// {
	// 	
	// 	InitialOffset = PlayerCameraBoom->SocketOffset;
	// }
	// UE_LOG(LogTemp, Warning, TEXT("Camera"));
	// if (GetPlayerCharacterFromActorInfo()->GetCameraMode() == PlayerCameraMode::FPPCamera)
	// {
	// 	GetWorld()->GetTimerManager().SetTimer(CameraBoomTimerHandle,
	// 	this, &UGA_Shouldering::UpdateCameraBoomOffset, 0.05f, true);
	// 	
	// }
}

// void UGA_Shouldering::UpdateCameraBoomOffset()
// {
// 	// CurrentOffset = PlayerCameraBoom->SocketOffset;
// 	// FVector NewOffset = FMath::VInterpTo(CurrentOffset, InitialOffset+AddOffset, GetWorld()->GetDeltaSeconds(), 30.f);  // 속도 조절
// 	// PlayerCameraBoom->SocketOffset = NewOffset;
// 	//
// 	// // 목표 위치에 도달하면 타이머 종료
// 	// if (FVector::Dist(NewOffset, CurrentOffset+AddOffset) < 1.0f)
// 	// {
// 	// 	GetWorld()->GetTimerManager().ClearTimer(CameraBoomTimerHandle);
// 	// }
// }

void UGA_Shouldering::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	
	if (GetPlayerCharacterFromActorInfo()->GetCameraMode() == PlayerCameraMode::FPPCamera)
	{
		PlayerCameraBoom->SetWanstReversePlaying(true);
		FVector OffsetDelta = FVector(110.f, 10.f, 0.f);
		float Duration = 0.2f;
		PlayerCameraBoom->TimelineAddOffset(OffsetDelta, Duration);
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	// // PlayerCameraBoom = GetPlayerCharacterFromActorInfo()->GetCameraBoom();
	// if (PlayerCameraBoom)
	// {
	// 	InitialOffset = PlayerCameraBoom->SocketOffset;
	// }
	// UE_LOG(LogTemp, Warning, TEXT("end"));
	// if (GetPlayerCharacterFromActorInfo()->GetCameraMode() == PlayerCameraMode::FPPCamera)
	// {
	// 	GetWorld()->GetTimerManager().SetTimer(CameraBoomTimerHandle,
	// 	this, &UGA_Shouldering::ReturnCameraBoomOffset, 0.05f, true);
	// 	
	// }
}

// void UGA_Shouldering::ReturnCameraBoomOffset()
// {
// 	// FVector NewOffset = FMath::VInterpTo(CurrentOffset, InitialOffset+SubOffset, GetWorld()->GetDeltaSeconds(), 30.f);  // 속도 조절
// 	//
// 	// PlayerCameraBoom->SocketOffset = NewOffset;
// 	//
// 	// // 목표 위치에 도달하면 타이머 종료
// 	// if (FVector::Dist(NewOffset, CurrentOffset+SubOffset) < 1.0f)
// 	// {
// 	// 	GetWorld()->GetTimerManager().ClearTimer(CameraBoomTimerHandle);
// 	// }
// }

