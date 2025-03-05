// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HUD/PlayerStatus/PlayerStatusWidget.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Character/PlayerCharacter.h"
#include "Components/ProgressBar.h"
#include "PlayerState/BasePlayerState.h"

void UPlayerStatusWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	// if(!PlayerCharacter)
	// {
	// 	return;
	// }
	//
	// ABasePlayerState* PS = PlayerCharacter->GetPlayerState<ABasePlayerState>();
	// if(!PS)
	// {
	// 	return;
	// }
	//
	// UBaseAbilitySystemComponent* ASC = Cast<UBaseAbilitySystemComponent>(PlayerCharacter->GetAbilitySystemComponent());
	// if(!ASC)
	// {
	// 	return;
	// }
	//
	// TArray<FGameplayAttribute> Array;
	//
	
	
}

void UPlayerStatusWidget::SetHealth(float OutHealth)
{
	//UE_LOG(LogTemp, Warning, TEXT("PlayerStatusWidget::SetHealth"));
	Health = OutHealth;

	SetProgressBar_Health(OutHealth);
}

void UPlayerStatusWidget::SetMaxHealth(float OutMaxHealth)
{
	MaxHealth = OutMaxHealth;

	SetProgressBar_Health(OutMaxHealth);
}

void UPlayerStatusWidget::SetStamina(float OutStamina)
{
	Stamina = OutStamina;

	SetProgressBar_Stamina(OutStamina);
}

void UPlayerStatusWidget::SetMaxStamina(float OutMaxStamina)
{
	MaxStamina = OutMaxStamina;

	SetProgressBar_Stamina(OutMaxStamina);
}

void UPlayerStatusWidget::SetProgressBar_Health(float OutHealth)
{
	if (OutHealth < 50.f)
	{
		ProgressBar_Health->SetFillColorAndOpacity(FLinearColor(1.0f, 0.759928f, 0.348958f, 0.8f));
	}
	
	if (OutHealth < 20.f)
	{
		ProgressBar_Health->SetFillColorAndOpacity(FLinearColor(1.0f, 0.0f, 0.008303f, 0.8f));
	}
	
	if (OutHealth < 10.f)
	{
		PlayAnimation(Anim_ProgressBar_Health,0.0f,0.0f);
	}
	else
	{
		StopAnimation(Anim_ProgressBar_Health);
	}
	
	ProgressBar_Health->SetPercent(Health / MaxHealth);
}


void UPlayerStatusWidget::SetProgressBar_Stamina(float OutStamina)
{
	ProgressBar_Stamina->SetPercent(Stamina / MaxStamina);
}


