// Fill out your copyright notice in the Description page of Project Settings.

#include "HelperMovement.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UHelperMovement::UHelperMovement()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	IsEnabled	  = false;
	Speed		  = 250.f;
	MoveVector	  = FVector(1.f, 0.f, 0.f);
	UpdatedVector = FVector(0.f);
}

// Called when the game starts
void UHelperMovement::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Owner = GetOwner();
	if (Owner)
	{
		SetEnabled(true);
	}
}

void UHelperMovement::SetEnabled(bool value)
{
	IsEnabled = value;
	if (value)
	{
		UpdatedVector = Owner->GetActorRotation().RotateVector(MoveVector);
	}
}

void UHelperMovement::SetMoveVector(FVector FWVector)
{
	MoveVector	  = FWVector;
	UpdatedVector = Owner->GetActorRotation().RotateVector(MoveVector);
}

// Called every frame
void UHelperMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (! IsEnabled)
		return;

	// ...
	if (AlwaysUpdateMoveVector)
	{
		UpdatedVector = Owner->GetActorRotation().RotateVector(MoveVector);
	}

	// Movement
	FVector nLocation;
	nLocation = Owner->GetActorLocation() + UpdatedVector * (Speed * DeltaTime);
	Owner->SetActorLocation(nLocation, true);
}
