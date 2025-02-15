// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayActor/Airplane/GA/GA_Airplane_Fall.h"

#include "Character/PlayerCharacter.h"
#include "Controller/BasePlayerController.h"
#include "GameInstance/BaseGameInstance.h"
#include "GameplayActor/Airplane/Airplane.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/AirplaneManager.h"

UGA_Airplane_Fall::UGA_Airplane_Fall()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

void UGA_Airplane_Fall::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                        const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	SetCanBeCanceled(false);

	UBaseGameInstance* GI = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (!GI)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	AAirplane* Airplane = GI->GetAirplaneManager()->SpawnedPlane;

	if (!Airplane)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);	
		return;		
	}
		
	APlayerCharacter* PlayerCharacter = GetPlayerCharacterFromActorInfo();
	ABasePlayerController* PlayerController = GetPlayerControllerFromActorInfo();

	PlayerCharacter->SetActorLocation(Airplane->GetActorLocation());
	PlayerCharacter->SetActorRotation(Airplane->GetActorRotation());
	
	PlayerController->SetViewTargetWithBlend(PlayerCharacter, 1.0f);
	PlayerController->Client_RemoveMappingContext();

	
	GetBaseAbilitySystemComponentFromActorInfo()->RemoveGrantedPlayerWeaponAbilities(Airplane->GetGrantedAbilitySpecHandles());
	//RemoveAirplaneAbility(Airplane);
	
	EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
}

void UGA_Airplane_Fall::RemoveAirplaneAbility_Implementation(AAirplane* Airplane)
{	
	GetBaseAbilitySystemComponentFromActorInfo()->RemoveGrantedPlayerWeaponAbilities(Airplane->GetGrantedAbilitySpecHandles());
}
