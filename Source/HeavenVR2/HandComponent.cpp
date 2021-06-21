// Fill out your copyright notice in the Description page of Project Settings.


#include "HandComponent.h"

void UHandComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	return;

}

AActor* UHandComponent::Grip()
{
	auto Held = Grabber->Grab(20.0f, true, true, EGrabTypeEnum::PRECISION_GRAB, "");
	if (Held)
	{
		FAttachmentTransformRules TransformRules = FAttachmentTransformRules(
			EAttachmentRule::SnapToTarget,
			true);
		Held->AttachToComponent(Hand, TransformRules, SocketName);
		return Held;
	}
	return nullptr;
}

AActor* UHandComponent::Release()
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

bool UHandComponent::IsPalmUp(bool Debug)
{
	if ((Hand->GetSocketRotation("").Roll <= -120.0f && Hand->GetSocketRotation("").Roll >= -180.0f) ||
		(Hand->GetSocketRotation("").Roll <= 180.0f && Hand->GetSocketRotation("").Roll >= 155.0f))
	{
		if (Debug)
		{
			UE_LOG(LogTemp, Error, TEXT("Can Inventory, %f"), Hand->GetSocketRotation("").Roll);
		}
		return true;
	}
	if (Debug)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't Inventory, %f"), Hand->GetSocketRotation("").Roll);
	}
	return false;
}
