// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "Interface/InteractInterface.h"
#include "VehicleBase.generated.h"

class UBoxComponent;
/**
 * 
 */
UCLASS()
class PUBG_API AVehicleBase : public AWheeledVehiclePawn, public IInteractInterface
{
	GENERATED_BODY()

	
	
protected:
	AVehicleBase();
	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite ,meta = (AllowPrivateAccess = "true"))  / AAwheeledVehiclePawn에 이미 메시있음
	// USkeletalMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* InteractionComponent;


	//카메라 관련
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, category = "Camera", meta = (AllowPrivateAccess = "true"))
	UPUBGSpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	

public:
	virtual FText LookAt() override;
	virtual void InteractWith_Implementation(APlayerCharacter* Character) override;
};
