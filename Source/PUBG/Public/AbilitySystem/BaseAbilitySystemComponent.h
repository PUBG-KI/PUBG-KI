// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "BaseAbilitySystemComponent.generated.h"

struct FPlayerAbilitySet;

/**
 * 
 */
UCLASS()
class PUBG_API UBaseAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	void OnAbilityInputPressed(const FGameplayTag& InputTag);
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void OnAbilityInputReleased(const FGameplayTag& InputTag);

	UFUNCTION(BlueprintCallable, Category = "Ability", meta = (Level = "1"))
	void GrantPlayerWeaponAbilities(const TArray<FPlayerAbilitySet>& WeaponAbilities, int32 Level, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void RemoveGrantedPlayerWeaponAbilities(UPARAM(ref) TArray<FGameplayAbilitySpecHandle>& SpecHandlesToRemove);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	bool TryActivateAbilityByTagToRandom(FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void TryCancelAbilityByTag(FGameplayTag Tag);

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, category = "Ability", meta = (AllowPrivateAccess = "true"))
	TArray<FGameplayAbilitySpecHandle> RecentGrantedAbilitySpecHandles;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "Ability", meta = (AllowPrivateAccess = "true"))
	bool bCancelInputReleased = true;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void GrantAbility(TSubclassOf<UPlayerGameplayAbility> AbilityToGrant, int32 Level, FGameplayAbilitySpecHandle& OutGrantedAbilitySpecHandles);


};
