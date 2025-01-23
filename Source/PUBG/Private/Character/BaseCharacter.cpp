// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"

// 무브먼트
#include "Component/Movement/PlayerMovementComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter(const class FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// 틱관련 기능 비활성화
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	BaseAbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>(TEXT("BaseAbilitySystemComponent"));
	BaseAttributeSet = CreateDefaultSubobject<UBaseAttributeSet>(TEXT("BaseAttributeSet"));
}

bool ABaseCharacter::IsAlive() const
{
	return GetHealth() > 0.0f;
}

float ABaseCharacter::GetHealth() const
{
	if (BaseAttributeSet.IsValid())
	{
		return BaseAttributeSet->GetHealth();
	}

	return 0.0f;
}

float ABaseCharacter::GetMaxHealth() const
{
	if (BaseAttributeSet.IsValid())
	{
		return BaseAttributeSet->GetMaxHealth();
	}

	return 0.0f;
}

float ABaseCharacter::GetStamina() const
{
	if (BaseAttributeSet.IsValid())
	{
		return BaseAttributeSet->GetStamina();
	}

	return 0.0f;
}

float ABaseCharacter::GetMaxStamina() const
{
	if (BaseAttributeSet.IsValid())
	{
		return BaseAttributeSet->GetMaxStamina();
	}

	return 0.0f;
}

float ABaseCharacter::GetMoveSpeed() const
{
	if (BaseAttributeSet.IsValid())
	{
		return BaseAttributeSet->GetMoveSpeed();
	}

	return 0.0f;
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return BaseAbilitySystemComponent.Get();
}

void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	
}

void ABaseCharacter::InitializeAttributes()
{
	if (!BaseAbilitySystemComponent.IsValid())
	{
		return;
	}

	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	// 서버와 클라이언트에서 실행 가능
	FGameplayEffectContextHandle EffectContext = BaseAbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = BaseAbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, 1, EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = BaseAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), BaseAbilitySystemComponent.Get());
	}
}

void ABaseCharacter::AddStartupEffects()
{
	if (GetLocalRole() != ROLE_Authority || !BaseAbilitySystemComponent.IsValid() || BaseAbilitySystemComponent->bStartupEffectsApplied)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = BaseAbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = BaseAbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = BaseAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), BaseAbilitySystemComponent.Get());
		}
	}
	BaseAbilitySystemComponent->bStartupEffectsApplied = true;
}

void ABaseCharacter::SetHealth(float Health)
{
	if (BaseAttributeSet.IsValid())
	 {
	     BaseAttributeSet->SetHealth(Health);
	 }
}

void ABaseCharacter::SetStamina(float Stamina)
{
	if (BaseAttributeSet.IsValid())
	{
		BaseAttributeSet->SetStamina(Stamina);
	}
}

void ABaseCharacter::SetMoveSpeed(float MoveSpeed)
{
	if (BaseAttributeSet.IsValid())
	{
		BaseAttributeSet->SetMoveSpeed(MoveSpeed);
	}
}



