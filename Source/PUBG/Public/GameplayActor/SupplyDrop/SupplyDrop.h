// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SupplyDrop.generated.h"

class UItemSpawnerComponent;
class AItemBase;
class UBoxComponent;

UCLASS()
class PUBG_API ASupplyDrop : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASupplyDrop();

	virtual void BeginPlay() override;	

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			   FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	FName GetRandomItemRowName();

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SpawnSupplyDropItem();
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite ,meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Smoke;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite ,meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* DropMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite ,meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Balloon;

	
	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystemComponent* ParticleComp;
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* ParticleEffect;
	
	FTimerHandle StopEffectHandle;

	//충돌체
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CollisionBoxComponent;

	//보급품 아이템
	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	// TArray<AItemBase*> SupplyDropItems;
	//
	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	// AItemBase* Item;
	//
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly , meta = (AllowPrivateAccess = "true"))
	// UChildActorComponent* ItemChildActorComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupplyDropItem",meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AItemBase> ItemClass;

	UPROPERTY()
	AItemBase* SpawnedItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UDataTable* ItemDataTable;

	//스폰컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner",meta = (AllowPrivateAccess = "true"))
	UItemSpawnerComponent* ItemSpawnerComponent;
};
