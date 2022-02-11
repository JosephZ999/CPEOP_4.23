// Fill out your copyright notice in the Description page of Project Settings.


#include "ShadowComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

#define CLASS_PATH "Blueprint'/Game/Blueprint/Objects/Dynamic/Shadow.Shadow_C'"

// Sets default values for this component's properties
UShadowComponent::UShadowComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Scale = 1.f;
	// ...

	// Init shadow static class

	ConstructorHelpers::FClassFinder<AActor> nObject(*FString(CLASS_PATH));
	if (nObject.Succeeded())
	{
		ShadowClass = nObject.Class;
	}
	IsBeingDestroyed();
}

void UShadowComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);
	DestroyShadow();
}

// Called when the game starts
void UShadowComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();

	if (ShadowClass)
	{
		ShadowObj = GetWorld()->SpawnActor<AActor>(ShadowClass, Owner->GetTransform());
		if (ShadowObj)
		{
			ShadowObj->SetActorScale3D(FVector(Scale));
			GetWorld()->GetTimerManager().SetTimer(nTimer, this, &UShadowComponent::SetupOwnerforShadow, 0.2f);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed creating shadow - %s"), *Owner->GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Shadow Class is not valid"));
	}
}


// Called every frame
void UShadowComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ShadowObj)
	{
		FVector nLoc = Owner->GetActorLocation();
		nLoc.Z = 0.f;
		ShadowObj->SetActorLocation(nLoc);
	}
	

	// ...
}

void UShadowComponent::SetupOwnerforShadow()
{
	ShadowObj->SetOwner(GetOwner());
}

void UShadowComponent::ShowShadow()
{
	if (ShadowObj)
	{
		ShadowObj->SetActorScale3D(FVector(Scale));
	}
}

void UShadowComponent::HideShadow()
{
	if (ShadowObj)
	{
		ShadowObj->SetActorScale3D(FVector(0.f));
	}
}

void UShadowComponent::DestroyShadow()
{
	if (ShadowObj)
	{
		ShadowObj->Destroy();
	}
}