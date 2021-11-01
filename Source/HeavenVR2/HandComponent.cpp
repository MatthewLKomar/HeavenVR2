// Fill out your copyright notice in the Description page of Project Settings.


#include "HandComponent.h"


AHandComponent::AHandComponent()
{
	PrimaryActorTick.bCanEverTick = true;
	//Init / Set up our hand root
	HandRoot = CreateDefaultSubobject<USceneComponent>(TEXT("HandRoot")); 
	HandRoot->SetupAttachment(GetRootComponent()); 

	//Init / Set up motion controller to hand root
	Controller = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Controller"));
	Controller->SetupAttachment(GetRootComponent());
	Controller->SetTrackingMotionSource(HandSide);

	//Init / Set up the grabber
	Grabber = CreateDefaultSubobject<UGrabber>(TEXT("Grabber"));
	Grabber->SetupAttachment(Controller);

	//Init / Set up the Hand Mesh
	HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hand"));
	HandMesh->SetupAttachment(Controller);
	if (HandSide == FXRMotionControllerBase::LeftHandSourceId)
	{
		HandMesh->SetRelativeRotation(FRotator(90.f, 0, 0));
		HandMesh->SetRelativeScale3D(FVector(1,1, -1));

	}
}
// Called every frame
void AHandComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called when the game starts or when spawned
void AHandComponent::BeginPlay()
{
	Super::BeginPlay();
	//TODO: Change this to work with EControllerHand and Controller->MotionSource	

}

AActor* AHandComponent::Grip()
{
	auto Held = Grabber->Grab(20.0f, true, true, EGrabTypeEnum::PRECISION_GRAB, "");
	if (Held)
	{
		FAttachmentTransformRules TransformRules = FAttachmentTransformRules(
			EAttachmentRule::SnapToTarget,
			true);
		Held->AttachToComponent(HandMesh, TransformRules, SocketName);
		return Held;
	}
	return nullptr;
}

AActor* AHandComponent::Release()
{
	auto Held = Grabber->Release();
	if (Held)
	{
		FDetachmentTransformRules TransformRules = FDetachmentTransformRules(
			EDetachmentRule::KeepWorld,
			true);
		Held->DetachFromActor(TransformRules);
		return Held;
	}
	return nullptr;
}

bool AHandComponent::IsPalmUp(bool Debug)
{
	if ((HandMesh->GetSocketRotation("").Roll <= -120.0f && HandMesh->GetSocketRotation("").Roll >= -180.0f) ||
		(HandMesh->GetSocketRotation("").Roll <= 180.0f && HandMesh->GetSocketRotation("").Roll >= 155.0f))
	{
		if (Debug)
		{
			UE_LOG(LogTemp, Error, TEXT("Can Inventory, %f"), HandMesh->GetSocketRotation("").Roll);
		}
		return true;
	}
	if (Debug)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't Inventory, %f"), HandMesh->GetSocketRotation("").Roll);
	}
	return false;
}
