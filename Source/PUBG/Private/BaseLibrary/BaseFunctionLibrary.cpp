// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseLibrary/BaseFunctionLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbility.h"
#include "Character/BaseCharacter.h"

UBaseAbilitySystemComponent* UBaseFunctionLibrary::NativeGetBaseAbilitySystemComponentFromActor(AActor* Actor)
{
	check(Actor);
	return CastChecked<UBaseAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor));
}

void UBaseFunctionLibrary::AddGameplayTagToActor(AActor* Actor, FGameplayTag Tag)
{
	UBaseAbilitySystemComponent* ASC = NativeGetBaseAbilitySystemComponentFromActor(Actor);
	if (!ASC->HasMatchingGameplayTag(Tag))
	{
		ASC->AddLooseGameplayTag(Tag);
	}
}

void UBaseFunctionLibrary::RemoveGameplayTagFromActor(AActor* Actor, FGameplayTag Tag)
{
	UBaseAbilitySystemComponent* ASC = NativeGetBaseAbilitySystemComponentFromActor(Actor);
	if (ASC->HasMatchingGameplayTag(Tag))
	{
		ASC->RemoveLooseGameplayTag(Tag);
	}
}

bool UBaseFunctionLibrary::ApplyDamageToActor(ABaseCharacter* SourceActor, ABaseCharacter* TargetActor,
	TSubclassOf<UGameplayEffect> DamageGameplayEffect, FHitResult& HitResult, float Damage)
{
	// if (!SourceActor || !TargetActor || !DamageGameplayEffect)
	// {
	// 	return false;
	// }
	//
	// FGameplayEffectSpecHandle DamageEffectSpecHandle = UGameplayAbility::MakeOutgoingGameplayEffectSpec(DamageGameplayEffect, UGameplayAbility::GetAbilityLevel());
	//
	// if(DamageEffectSpecHandle.IsValid())
	// {
	// 	DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), Damage);
	//
	// 	FGameplayEffectContextHandle EffectContext = SourceActor->GetAbilitySystemComponent()->MakeEffectContext();
	// 	if(EffectContext.IsValid())
	// 	{
	// 		EffectContext.AddInstigator(SourceActor, SourceActor); // 소스 액터를 Instigator로 추가
	// 		EffectContext.AddHitResult(HitResult);   // 타겟 액터를 포함하는 HitResult 추가
	//
	// 		// Effect Context를 Spec에 설정
	// 		DamageEffectSpecHandle.Data.Get()->SetContext(EffectContext);
	// 		SourceActor->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*DamageEffectSpecHandle.Data.Get(), TargetActor->GetAbilitySystemComponent());
	//
	// 		return true;
	// 	}
	// }
	//
	 return false;
}

bool UBaseFunctionLibrary::NativeActorHasTag(AActor* Actor, FGameplayTag Tag)
{
	UBaseAbilitySystemComponent* ASC = NativeGetBaseAbilitySystemComponentFromActor(Actor);
	return ASC->HasMatchingGameplayTag(Tag);
}

void UBaseFunctionLibrary::BP_HasTag(AActor* Actor, FGameplayTag Tag, EBaseConfirmType& OutType)
{
	OutType = NativeActorHasTag(Actor, Tag) ? EBaseConfirmType::Yes : EBaseConfirmType::No;
}

bool UBaseFunctionLibrary::HasTag(AActor* Actor, FGameplayTag Tag)
{UBaseAbilitySystemComponent* ASC = NativeGetBaseAbilitySystemComponentFromActor(Actor);
	return ASC->HasMatchingGameplayTag(Tag);
}
