// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Interaction/GA_Interaction.h"

#include "Character/PlayerCharacter.h"
#include "Interface/InteractInterface.h"

void UGA_Interaction::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (AActor* LookAtActor = Cast<AActor>(GetPlayerCharacterFromActorInfo()->GetLookAtActor()))
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *LookAtActor->GetActorLabel());

		if (LookAtActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
		{
			IInteractInterface::Execute_InteractWith(LookAtActor, GetPlayerCharacterFromActorInfo());
		}
		IInteractInterface* InteractInterface = Cast<IInteractInterface>(LookAtActor);
		InteractInterface->InteractWith(GetPlayerCharacterFromActorInfo());
	}

	EndAbility(Handle,ActorInfo, ActivationInfo, false, true);
}
