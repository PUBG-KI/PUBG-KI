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
	UE_LOG(LogTemp, Warning, TEXT("PlayerStatusWidget::SetHealth"));
	Health = OutHealth;

	SetProgressBar_Health(OutHealth);
}

void UPlayerStatusWidget::SetMaxHealth(float OutMaxHealth)
{
	MaxHealth = OutMaxHealth;

	SetProgressBar_Health(OutMaxHealth);
}

void UPlayerStatusWidget::SetProgressBar_Health(float OutHealth)
{
	if (OutHealth < 50)
	{
		ProgressBar_Health->SetFillColorAndOpacity(FLinearColor(1.0f, 0.759928f, 0.348958f, 0.8f));
	}
	
	if (OutHealth < 20)
	{
		ProgressBar_Health->SetFillColorAndOpacity(FLinearColor(1.0f, 0.0f, 0.008303f, 0.8f));
	}
	
	if (OutHealth < 10)
	{
		PlayAnimation(Anim_ProgressBar_Health,0.0f,0.0f);
	}
	ProgressBar_Health->SetPercent(Health / MaxHealth);
}
