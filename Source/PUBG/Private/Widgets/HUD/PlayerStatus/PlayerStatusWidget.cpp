// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HUD/PlayerStatus/PlayerStatusWidget.h"

#include "Components/ProgressBar.h"

void UPlayerStatusWidget::SetHealth(float OutHealth)
{
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
	ProgressBar_Health->SetPercent(Health / MaxHealth);
}
