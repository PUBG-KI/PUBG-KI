// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractInterface.h"
#include "Components/TimelineComponent.h"

#include "InteractDoor.generated.h"

class UBoxComponent;
class APlayerCharacter;

UCLASS()
class PUBG_API AInteractDoor : public AActor , public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractDoor();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite ,meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* DoorMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* DoorCollisionBox;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* InteractionTriggerBox;

	virtual void InteractWith_Implementation(APlayerCharacter* Character) override;
	virtual FText LookAt() override; 

	bool IsOpen = false;

	

protected:
	
	FTimeline TimeLine;
	
	UPROPERTY(EditAnywhere)
	UCurveFloat* CurveFloat;

	UPROPERTY(EditAnywhere)
	float DoorRotateAngle = 90.0f;

	bool bDoorOnSameSide;

	UFUNCTION()
	void OpenDoor(float Value); 

	void SetDoorOnSameSide(APlayerCharacter* PlayerCharacter);
};
