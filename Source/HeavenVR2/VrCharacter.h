// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/Engine.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/TargetPoint.h"
#include "MotionControllerComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "VrCharacter.generated.h"

UCLASS()
class HEAVENVR2_API AVRChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVRChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class USceneComponent* VRRoot = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UCameraComponent* Camera = nullptr;

	UFUNCTION(BlueprintCallable)
		void MoveForward(float Value);
	UFUNCTION(BlueprintCallable)
		void MoveRight(float Value);

	UFUNCTION(BlueprintCallable)
		void GripLeft();

	UFUNCTION(BlueprintCallable)
		void ReleaseLeft();

	UFUNCTION(BlueprintCallable)
		void GripRight();

	UFUNCTION(BlueprintCallable)
		void ReleaseRight();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		AActor* HeldObject = nullptr;

	UPROPERTY(BlueprintReadWrite)
		FName SocketName = "GripSocket";
	UPROPERTY(BlueprintReadWrite)
		FName FingerTip = "Pointer";

	UFUNCTION(BlueprintCallable)
		bool IsHeadsetOn();

	UFUNCTION(BlueprintCallable)
		void SetPlayerTracking();

	void CollisionOffset();

	UPROPERTY(BlueprintReadWrite)
		AActor* ItemInHand;
private:

private:
	FVector CameraHeight = FVector(0.f, 0.f, 88.f);
	FVector NewCameraOffset = FVector(0.0f, 0.0f, 0.0f);


};
