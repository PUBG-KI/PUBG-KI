// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Interaction/GA_Interaction.h"

#include "Character/PlayerCharacter.h"
#include "Interface/InteractInterface.h"

UGA_Interaction::UGA_Interaction()
{
	//NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
	//NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalOnly;

}

void UGA_Interaction::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Warning, TEXT("GA_Interaction"));

	if (GetPlayerCharacterFromActorInfo()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Server : GA_Interaction"));

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Execute Client : GA_Interaction"));

	}
	
	if (AActor* LookAtActor = Cast<AActor>(GetPlayerCharacterFromActorInfo()->GetLookAtActor()))
	{
		if (LookAtActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
		{
			IInteractInterface::Execute_InteractWith(LookAtActor, GetPlayerCharacterFromActorInfo());
		}

		// if (AActor* LookAtActor = Cast<AActor>(GetPlayerCharacterFromActorInfo()->GetLookAtActor()))
		// {
		// 	// 인터페이스 구현이 있는지 확인
		// 	if (IInteractInterface* InteractInterface = Cast<IInteractInterface>(LookAtActor))
		// 	{
		// 		// 인터페이스 메서드 호출
		// 		InteractInterface->InteractWith(GetPlayerCharacterFromActorInfo());
		// 	}
		// }

	}

	EndAbility(Handle,ActorInfo, ActivationInfo, false, true);
}
