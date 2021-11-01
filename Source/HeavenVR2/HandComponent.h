// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionControllerComponent.h" //Parent class
#include "Components/StaticMeshComponent.h" // Hand
#include "Grabber.h" //Grabber
#include "GameFramework/Actor.h" //Actor reference
#include "XRMotionControllerBase.h"
#include "HandComponent.generated.h"

/**
 * This is the HandComponent class. This is a motioncontroller component with a bunch of
 * tidbits to make this more generic and useful! 
 */
UCLASS(BlueprintType)
class HEAVENVR2_API AHandComponent : public AActor
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	//constructor
	AHandComponent();

	//vars
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class USceneComponent* HandRoot = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UMotionControllerComponent* Controller;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UGrabber* Grabber;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USkeletalMeshComponent* HandMesh;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName HandSide = FXRMotionControllerBase::LeftHandSourceId;
	//Functions
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		AActor* Grip();
	UFUNCTION(BlueprintCallable)
		AActor* Release();
	UFUNCTION(BlueprintCallable)
		bool IsPalmUp(bool Debug = false);
};
