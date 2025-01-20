// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Inventory/GA_ToggleInventory.h"

#include "AbilitySystem/AbilityTask/FindFarmingItem.h"
#include "Component/NearArea/NearComponent.h"
#include "Item/ItemBase.h"
#include "Kismet/KismetSystemLibrary.h"

void UGA_ToggleInventory::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
										  const FGameplayAbilityActorInfo* ActorInfo,
										  const FGameplayAbilityActivationInfo ActivationInfo,
										  const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UInventoryWidget* InventoryWidget = GetPlayerCharacterFromActorInfo()->GetInventoryWidget();
	if (InventoryWidget != nullptr)
	{
		
		// 인벤토리가 켜져있으면 끄기
		if (InventoryWidget->GetVisibility() == ESlateVisibility::Visible)
		{
			InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
			GetPlayerCharacterFromActorInfo()->InputModeGame();

			TickTask = UFindFarmingItem::ExecuteTaskTick(this);
			
			// TickTask 종료 처리
			if (TickTask)
			{
				TickTask->OnAbilityTaskTick.RemoveDynamic(this, &ThisClass::FindFarmingItem);
				TickTask->EndTask();
				TickTask = nullptr;  // null로 초기화
			}

			EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		}
		// 인벤토리가 꺼져있으면 켜기
		else if (InventoryWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			InventoryWidget->SetVisibility(ESlateVisibility::Visible);
			GetPlayerCharacterFromActorInfo()->InputModeUI();

			TickTask = UFindFarmingItem::ExecuteTaskTick(this);
			
			// 새로운 TickTask 생성
			if (TickTask)
			{
				// TickTask->EndTask();  // 기존 Task 종료
				// TickTask = nullptr;
				TickTask->SetWaitingOnAvatar();  // Ability와 Task의 수명 연동
				TickTask->OnAbilityTaskTick.AddDynamic(this, &ThisClass::FindFarmingItem);
				TickTask->ReadyForActivation();
			}
		}
	}
}


void UGA_ToggleInventory::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_ToggleInventory::FindFarmingItem(float DeltaTime)
{
	//GetPlayerCharacterFromActorInfo()->GetFarmingAbleItem().Empty();
	
	EObjectTypeQuery TraceType = UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1);
	BoxTraceChannel.Add(TraceType);
	
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
		if (AActor* HitActor=HitResult.GetActor())
		{
			if (AItemBase* ItemBase = Cast<AItemBase>(HitActor))
			{
				Items.Add(ItemBase);
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
