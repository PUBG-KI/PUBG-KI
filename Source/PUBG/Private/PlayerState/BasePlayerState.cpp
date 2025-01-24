// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/BasePlayerState.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Character/PlayerCharacter.h"
#include "Controller/BasePlayerController.h"

// 위젯 추가
//#include "UI/HUDWidget.h"

ABasePlayerState::ABasePlayerState()
{
	// 능력 시스템 구성요소를 생성하고 명시적으로 복제되도록 설정합니다.
	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	// 혼합 모드는 GE가 시뮬레이션된 프록시에 복제되지 않고 GE만 자신에게 복제된다는 의미입니다. 다른 GDPlayerState(Hero)가 GE를 수신하면,
	// 서버에서는 이에 대해 알려주지 않습니다. 속성, GameplayTags 및 GameplayCue는 계속해서 복제됩니다.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	// 속성 세트를 생성합니다. 이는 기본적으로 복제됩니다.
	// AbilitySystemComponent를 소유한 액터의 하위 객체로 추가
	// AbilitySystemComponent에 AttributeSet을 자동으로 등록합니다.
	AttributeSetBase = CreateDefaultSubobject<UBaseAttributeSet>(TEXT("AttributeSetBase"));

	// PlayerState의 NetUpdateFrequency를 캐릭터와 동일하게 설정합니다.
	// PlayerStates의 기본값은 매우 낮으며 능력 시스템에 지연이 발생합니다.
	// 100은 출시 게임에 비해 너무 높을 수 있으므로 필요에 맞게 조정할 수 있습니다.
	NetUpdateFrequency = 100.0f;

	// Cache tags
	//DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

class UAbilitySystemComponent* ABasePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

class UBaseAttributeSet* ABasePlayerState::GetAttributeSetBase() const
{
	return AttributeSetBase;
}

bool ABasePlayerState::IsAlive() const
{
	return GetHealth() > 0.0f;
}

float ABasePlayerState::GetHealth() const
{
	return AttributeSetBase->GetHealth();
}

float ABasePlayerState::GetMaxHealth() const
{
	return AttributeSetBase->GetMaxHealth();
}

float ABasePlayerState::GetHealthRegenRate() const
{
	return AttributeSetBase->GetHealthRegenRate();
}

float ABasePlayerState::GetStamina() const
{
	return AttributeSetBase->GetStamina();
}

float ABasePlayerState::GetMaxStamina() const
{
	return AttributeSetBase->GetMaxStamina();
}

float ABasePlayerState::GetStaminaRegenRate() const
{
	return AttributeSetBase->GetStaminaRegenRate();
}

float ABasePlayerState::GetArmor() const
{
	return AttributeSetBase->GetArmor();
}

float ABasePlayerState::GetMoveSpeed() const
{
	return AttributeSetBase->GetMoveSpeed();
}

void ABasePlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void ABasePlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
	float Health = Data.NewValue;
	
	// Update floating status bar
	//APlayerCharacter* Player = Cast<APlayerCharacter>(GetPawn());
	//if (Player)
	//{
	//	UFloatingStatusBarWidget* PlayerFloatingStatusBar = Player->GetFloatingStatusBar();
	//	if (PlayerFloatingStatusBar)
	//	{
	//		PlayerFloatingStatusBar->SetHealthPercentage(Health / GetMaxHealth());
	//	}
	//}

	// Update the HUD
	// Handled in the UI itself using the AsyncTaskAttributeChanged node as an example how to do it in Blueprint

	// If the player died, handle death
	//if (!IsAlive() && !AbilitySystemComponent->HasMatchingGameplayTag(DeadTag))
	//{
	//	if (Player)
	//	{
	//		Player->Die();
	//	}
	//}
}

void ABasePlayerState::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	float MaxHealth = Data.NewValue;

	// Update floating status bar
	//APlayerCharacter* Player = Cast<APlayerCharacter>(GetPawn());
	//if (Player)
	//{
	//	UFloatingStatusBarWidget* PlayerFloatingStatusBar = Player->GetFloatingStatusBar();
	//	if (PlayerFloatingStatusBar)
	//	{
	//		PlayerFloatingStatusBar->SetHealthPercentage(GetHealth() / MaxHealth);
	//	}
	//}

	//// Update the HUD
	//ABasePlayerController* PC = Cast<ABasePlayerController>(GetOwner());
	//if (PC)
	//{
	//	UGDHUDWidget* HUD = PC->GetHUD();
	//	if (HUD)
	//	{
	//		HUD->SetMaxHealth(MaxHealth);
	//	}
	//}
}

void ABasePlayerState::HealthRegenRateChanged(const FOnAttributeChangeData& Data)
{
	float HealthRegenRate = Data.NewValue;

	// Update the HUD
	//ABasePlayerController* PC = Cast<ABasePlayerController>(GetOwner());
	//if (PC)
	//{
	//	UHUDWidget* HUD = PC->GetHUD();
	//	if (HUD)
	//	{
	//		HUD->SetHealthRegenRate(HealthRegenRate);
	//	}
	//}
}

void ABasePlayerState::StaminaChanged(const FOnAttributeChangeData& Data)
{	
	float Stamina = Data.NewValue;
	
	// Update the HUD
	// ABasePlayerController* PC = Cast<ABasePlayerController>(GetOwner());
	// if (PC)
	// {
	// 	UHUDWidget* HUD = PC->GetHUD();
	// 	if (HUD)
	// 	{
	// 		HUD->SetMaxStamina(Stamina);
	// 	}
	// }
}

void ABasePlayerState::MaxStaminaChanged(const FOnAttributeChangeData& Data)
{
	float MaxStamina = Data.NewValue;

	// Update floating status bar
	//APlayerCharacter* Player = Cast<APlayerCharacter>(GetPawn());
	//if (Player)
	//{
	//	UFloatingStatusBarWidget* PlayerFloatingStatusBar = Player->GetFloatingStatusBar();
	//	if (PlayerFloatingStatusBar)
	//	{
	//		PlayerFloatingStatusBar->SetHealthPercentage(GetStamina() / MaxStamina);
	//	}
	//}
	// Update the HUD
	// ABasePlayerController* PC = Cast<ABasePlayerController>(GetOwner());
	// if (PC)
	// {
	// 	UHUDWidget* HUD = PC->GetHUD();
	// 	if (HUD)
	// 	{
	// 		HUD->SetMaxStamina(MaxStamina);
	// 	}
	// }
}

void ABasePlayerState::StaminaRegenRateChanged(const FOnAttributeChangeData& Data)
{
	float StaminaRegenRate = Data.NewValue;

	// Update the HUD
	// ABasePlayerController* PC = Cast<ABasePlayerController>(GetOwner());
	// if (PC)
	// {
	// 	UHUDWidget* HUD = PC->GetHUD();
	// 	if (HUD)
	// 	{
	// 		HUD->SetStaminaRegenRate(StaminaRegenRate);
	// 	}
	// }
}
