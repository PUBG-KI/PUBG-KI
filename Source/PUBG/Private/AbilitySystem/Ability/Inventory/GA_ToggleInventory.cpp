// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Inventory/GA_ToggleInventory.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AbilityTask/FindFarmingItem.h"
#include "Component/NearArea/NearComponent.h"
#include "Item/ItemBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseGameplayTag.h"



void UGA_ToggleInventory::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    UInventoryWidget* InventoryWidget = GetPlayerCharacterFromActorInfo()->GetInventoryWidget();
    if (InventoryWidget != nullptr)
    {
    	if (TickTask == nullptr)
    	{
    		TickTask = UFindFarmingItem::ExecuteTaskTick(this);
    		TickTask->OnAbilityTaskTick.RemoveDynamic(this, &ThisClass::FindFarmingItem);
    		TickTask->OnAbilityTaskTick.AddDynamic(this, &ThisClass::FindFarmingItem);
    		TickTask->SetWaitingOnAvatar();  // Ability와 Task의 수명 연동
    	}
    	
    	if (InventoryWidget->GetVisibility() == ESlateVisibility::Collapsed)
    	{
    		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
    		GetPlayerCharacterFromActorInfo()->InputModeUI();
    
    		// 새로운 TickTask 생성
    		if (TickTask)
    		{
    			UE_LOG(LogTemp, Warning, TEXT("TickTask->OnAbilityTaskTick1"));
    			TickTask->ReadyForActivation();
    		}
	
    		//EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
    		//CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
    	}
    	
    	// if (TickTask == nullptr)
    	// {
	    //     TickTask = UFindFarmingItem::ExecuteTaskTick(this);
    	// 	TickTask->OnAbilityTaskTick.RemoveDynamic(this, &ThisClass::FindFarmingItem);
    	// 	TickTask->OnAbilityTaskTick.AddDynamic(this, &ThisClass::FindFarmingItem);
    	// 	TickTask->SetWaitingOnAvatar();  // Ability와 Task의 수명 연동
    	// }
        
        // 인벤토리가 켜져있으면 끄기
        // if (InventoryWidget->GetVisibility() == ESlateVisibility::Visible)
        // {
        //     InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
        //     GetPlayerCharacterFromActorInfo()->InputModeGame();
        //
        //     // 기존 TickTask 종료 처리
        //     if (TickTask)
        //     {
        //     	UE_LOG(LogTemp, Warning, TEXT("TickTask->OnAbilityTaskTick"));
        //     	TickTask->OnAbilityTaskTick.RemoveDynamic(this, &ThisClass::FindFarmingItem);
        //         TickTask->EndTask();
        //         TickTask = nullptr;  // null로 초기화
        //     }
        //
        //     EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
        // }
        // // 인벤토리가 꺼져있으면 켜기
        // else if (InventoryWidget->GetVisibility() == ESlateVisibility::Collapsed)
        // {
        //     InventoryWidget->SetVisibility(ESlateVisibility::Visible);
        //     GetPlayerCharacterFromActorInfo()->InputModeUI();
        //     
        //     // 새로운 TickTask 생성
        //     if (TickTask)
        //     {
        //     	UE_LOG(LogTemp, Warning, TEXT("TickTask->OnAbilityTaskTick1"));
        //         TickTask->ReadyForActivation();
        //     }
        // 	
        // 	//EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
        // 	//CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
        //
        // }
    }
}

void UGA_ToggleInventory::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	UInventoryWidget* InventoryWidget = GetPlayerCharacterFromActorInfo()->GetInventoryWidget();
	if (InventoryWidget != nullptr)
	{
		if (InventoryWidget->GetVisibility() == ESlateVisibility::Visible)
		{
			InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
			GetPlayerCharacterFromActorInfo()->InputModeGame();
			// 기존 TickTask 종료 처리
			if (TickTask)
			{
				UE_LOG(LogTemp, Warning, TEXT("TickTask->OnAbilityTaskTick"));
				TickTask->OnAbilityTaskTick.RemoveDynamic(this, &ThisClass::FindFarmingItem);
				TickTask->EndTask();
				TickTask = nullptr;  // null로 초기화
			}
			EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		}
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


void UGA_ToggleInventory::FindFarmingItem(float DeltaTime)
{

	//EObjectTypeQuery TraceType = UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1);
	//BoxTraceChannel.Add(TraceType);
	
	EObjectTypeQuery TraceType1 = UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2);
	BoxTraceChannel.Add(TraceType1);
	
	TArray<FHitResult> HitResults;
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetPlayerCharacterFromActorInfo(),
		GetPlayerCharacterFromActorInfo()->GetActorLocation(),
		//GetPlayerCharacterFromActorInfo()->GetActorLocation()+GetPlayerCharacterFromActorInfo()->GetActorForwardVector()* TraceDistance,
		GetPlayerCharacterFromActorInfo()->GetActorLocation(),
		TraceBoxSize/ 2,
		GetPlayerCharacterFromActorInfo()->GetActorForwardVector().ToOrientationRotator(),
		BoxTraceChannel,
		false,
		TArray<AActor*>(),
		//bIsDebugging?EDrawDebugTrace::ForDuration:EDrawDebugTrace::None,
		EDrawDebugTrace::ForDuration,
		HitResults,
		true
		);

	UNearComponent* NearComponent = Cast<UNearComponent>(GetPlayerCharacterFromActorInfo()->GetNearComponent());

	TArray<AItemBase*> Items;
	bool bShouldUpdate = true;
	
	for (const FHitResult& HitResult : HitResults)
	{
		if (AActor* HitActor = HitResult.GetActor())
		{
			UE_LOG(LogTemp, Warning, TEXT("HitActor : %s"), *HitActor->GetName());
			if (AItemBase* ItemBase = Cast<AItemBase>(HitActor))
			{
				Items.Add(ItemBase);
				UE_LOG(LogTemp, Warning, TEXT("Item Count : %d"), Items.Num());
			}
		}
	}
	
	for ( AItemBase* Item : Items)
	{
		bShouldUpdate = NearComponent->ShouldUpdate(Item);
		if (bShouldUpdate == false)
		{
			NearComponent->GetGroundItem().Empty();
			break;
		}
	}
	
	if (bShouldUpdate == true)
	{
		return;
	}
	else
	{
		for ( AItemBase* Item : Items)
		{
			if ( NearComponent->ShouldUpdate(Item) == false)
			{
				NearComponent->GetGroundItem().Add(Item);
			}
		}
		NearComponent->UpdateInventory();
	}
}
