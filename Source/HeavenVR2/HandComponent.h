// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionControllerComponent.h" //Parent class
#include "Components/StaticMeshComponent.h" // Hand
#include "Grabber.h" //Grabber
#include "GameFramework/Actor.h" //Actor reference
#include "HandComponent.generated.h"

/**
 * This is the HandComponent class. This is a motioncontroller component with a bunch of
 * tidbits to make this more generic and useful! 
 */
UCLASS()
class HEAVENVR2_API UHandComponent : public UMotionControllerComponent
{
	GENERATED_BODY()
public:
	//vars
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UMotionControllerComponent* Controller;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UGrabber* Grabber;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USkeletalMeshComponent* Hand;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		AActor* HeldObject = nullptr;
	UPROPERTY(BlueprintReadWrite)
		FName SocketName = "GripSocket";
	UPROPERTY(BlueprintReadWrite)
		bool GripPressed = false;
	UPROPERTY(BlueprintReadWrite)
		bool TriggerPressed = false;
	UPROPERTY(BlueprintReadWrite)
		AActor* ItemInHand;

	//Functions
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		AActor* Grip();
	UFUNCTION(BlueprintCallable)
		AActor* Release();
	UFUNCTION(BlueprintCallable)
		bool IsPalmUp(bool Debug = false);
};
