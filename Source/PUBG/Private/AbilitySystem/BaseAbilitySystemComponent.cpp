// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/Ability/PlayerGameplayAbility.h"
#include "BaseLibrary/BaseStructType.h"
#include "GameplayAbilitySpec.h"
#include "AbilitySystem/BaseGameplayTag.h"


void UBaseAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (!Spec.DynamicAbilityTags.HasTagExact(InputTag)) continue;

		if (InputTag.MatchesTag(BaseGameplayTag::InputTag_Toggle))
		{
			//InputTag_Toggle 태그가 있으면 Toggle On/Off 처리 
			if (Spec.IsActive())
			{
				CancelAbilityHandle(Spec.Handle);
			}
			else
			{
				TryActivateAbility(Spec.Handle);
			}
		}
		else
		{
			TryActivateAbility(Spec.Handle);
		}
	}
}

void UBaseAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}
	
	for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			CancelAbilityHandle(Spec.Handle);
		}
	}
}

void UBaseAbilitySystemComponent::GrantPlayerWeaponAbilities(const TArray<FPlayerAbilitySet>& WeaponAbilities,
	int32 Level, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles)
{
	if (WeaponAbilities.IsEmpty())
	{
		return;
	}

	for (const FPlayerAbilitySet& WeaponAbilitySet : WeaponAbilities)
	{
		if (!WeaponAbilitySet.IsValid()) continue;

		FGameplayAbilitySpec Spec(WeaponAbilitySet.AbilityToGrant);
		Spec.SourceObject = GetAvatarActor();
		Spec.Level = Level;
		Spec.DynamicAbilityTags.AddTag(WeaponAbilitySet.InputTag);
		OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(Spec));
	}
}

void UBaseAbilitySystemComponent::RemoveGrantedPlayerWeaponAbilities(
	TArray<FGameplayAbilitySpecHandle>& SpecHandlesToRemove)
{	
	if (SpecHandlesToRemove.IsEmpty())
	{
		return;
	}

	for (FGameplayAbilitySpecHandle& SpecHandle : SpecHandlesToRemove)
	{

		if (SpecHandle.IsValid())
		{
			ClearAbility(SpecHandle);
		}
	}

	SpecHandlesToRemove.Empty();
}

bool UBaseAbilitySystemComponent::TryActivateAbilityByTagToRandom(FGameplayTag Tag)
{
	check(Tag.IsValid());

	TArray<FGameplayAbilitySpec*> AbilitySpecs;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(Tag.GetSingleTagContainer(), AbilitySpecs);

	if (!AbilitySpecs.IsEmpty())
	{
		const int32 RandomAbilityIndex = FMath::RandRange(0, AbilitySpecs.Num() - 1);
		FGameplayAbilitySpec* AbilitySpec = AbilitySpecs[RandomAbilityIndex];

		check(AbilitySpec);

		if (!AbilitySpec->IsActive())
		{
			return TryActivateAbility(AbilitySpec->Handle);
		}
	}
		
	return false;
}

void UBaseAbilitySystemComponent::TryCancelAbilityByTag(FGameplayTag Tag)
{
	check(Tag.IsValid());
	FGameplayTagContainer GameplayTagContainer = Tag.GetSingleTagContainer();
	CancelAbilities(&GameplayTagContainer);
}

void UBaseAbilitySystemComponent::GrantAbility(TSubclassOf<UPlayerGameplayAbility> AbilityToGrant, int32 Level,
	FGameplayAbilitySpecHandle& OutGrantedAbilitySpecHandles)
{
	FGameplayAbilitySpec Spec(AbilityToGrant);
	Spec.SourceObject = GetAvatarActor();
	Spec.Level = Level;
	OutGrantedAbilitySpecHandles = GiveAbility(Spec);
}

void UBaseAbilitySystemComponent::ReceiveDamage(UBaseAbilitySystemComponent* SourceASC, float UnmitigatedDamage,
	float MitigatedDamage)
{
	ReceivedDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
}

