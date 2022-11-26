// Fill out your copyright notice in the Description page of Project Settings.


#include "VrCharacter.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "IHeadMountedDisplay.h"
#include "XRMotionControllerBase.h"
#include "Components/SceneComponent.h"
#include <exception>

// Sets default values
AVRChar::AVRChar()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot")); //Creating VrOrigin Scene Component
	VRRoot->SetupAttachment(GetRootComponent()); //Attaching VrOrigin to root component (this case a capsule component)

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera")); //Creating the camera
	Camera->SetupAttachment(VRRoot); //attaches camera to Vr Origin
	FVector CameraOffset = FVector(0, 0, -(GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	//VRRoot->SetRelativeLocation(CameraOffset);
}

void AVRChar::SetPlayerTracking()
{
	FName Headset = UHeadMountedDisplayFunctionLibrary::GetHMDDeviceName();
	if (Headset == "OculusHMD" || Headset == "SteamVR" || Headset == "WindowsMixedRealityHMD")
	{
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
	}
	else
	{
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Eye);
		VRRoot->AddLocalOffset(CameraHeight);
	}
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AVRChar::CollisionOffset()
{
	//Calculations to deal with collison. This basically prevents us from walking through walls
	NewCameraOffset = Camera->GetComponentLocation() - GetActorLocation();
	NewCameraOffset.Z = 0;
	AddActorWorldOffset(NewCameraOffset);
	VRRoot->AddWorldOffset(-NewCameraOffset);
}


// Called when the game starts or when spawned
void AVRChar::BeginPlay()
{
	Super::BeginPlay();
	SetPlayerTracking();
	//Target Point set up stuff
	UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), ATargetPoint::StaticClass(), TargetPointsFound);
	if (TargetPointsFound.Num() >= 1) //need to do a null check to make sure we have an array, else we might crash
	{
		CurrentLocation = StartLocation;
		MaxLocations = TargetPointsFound.Num() - 1;
		MoveToPoint(TargetPointsFound[StartLocation]);
	}
}



// Called every frame
void AVRChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//CollisionOffset();
}

// Called to bind functionality to input
void AVRChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TEXT("NextPoint"), IE_Pressed, this, &AVRChar::ToNextPoint);
	PlayerInputComponent->BindAction(TEXT("PreviousPoint"), IE_Pressed, this, &AVRChar::ToPreviousPoint);

	PlayerInputComponent->BindAxis(TEXT("NextForward"), this, &AVRChar::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AVRChar::MoveRight);
}


void AVRChar::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AVRChar::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AVRChar::MoveToPoint(AActor* PointOfInterst)
{
	if (PointOfInterst)	SetActorLocation(PointOfInterst->GetActorLocation());
}


void AVRChar::ToNextPoint()
{
	//to avoid array out of bounds error
	if (MaxLocations == 0) return;
	//Move forward in the array of target points
	CurrentLocation += 1;
	//if at [end of the array] go to [0]
	if (CurrentLocation > MaxLocations)
	{
		CurrentLocation = 0;
	}
	//Teleport to currently selected target point
	MoveToPoint(TargetPointsFound[CurrentLocation]);
	UE_LOG(LogTemp, Warning, TEXT("Moving to next point"));

}

void AVRChar::ToPreviousPoint()
{
	//Move back in the array of target points
	CurrentLocation -= 1;
	//if at [0] go to [end of the array]
	if (CurrentLocation < 0)
	{
		CurrentLocation = MaxLocations;
	}
	//Teleport to currently selected target point
	MoveToPoint(TargetPointsFound[CurrentLocation]);
	UE_LOG(LogTemp, Warning, TEXT("Moving to previous point"));

}


void AVRChar::GripLeft()
{
}

void AVRChar::ReleaseLeft()
{
}

void AVRChar::GripRight()
{
}

void AVRChar::ReleaseRight()
{
}

bool AVRChar::IsHeadsetOn()
{
	return false;
}


