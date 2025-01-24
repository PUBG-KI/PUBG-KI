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

FString UBaseFunctionLibrary::ReadFile(FString FilePath)
{
	FString DirectoryPath = FPaths::ProjectContentDir();
	FString FullPath = DirectoryPath + "/" + FilePath;
	FString Result;
	IPlatformFile& File = FPlatformFileManager::Get().GetPlatformFile();

	if(File.FileExists(*FullPath))
	{
		FFileHelper::LoadFileToString(Result,*FullPath);
	}

	return Result;
}
