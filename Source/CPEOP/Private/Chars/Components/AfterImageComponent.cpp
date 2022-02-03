// Fill out your copyright notice in the Description page of Project Settings.


#include "AfterImageComponent.h"
#include "PaperFlipbook.h"
#include "Objects/Dynamic/AfterImage.h"
#include "Engine/World.h"

UAfterImageComponent::UAfterImageComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UAfterImageComponent::InitClass()
{

}

void UAfterImageComponent::Create(FAfterImageStruct & AfterImage, float Time)
{
	if (Time > 0.f)
	{
		ImageData.Add(AfterImage);
		FTimerHandle Timer;
		GetOwner()->GetWorldTimerManager().SetTimer(Timer, this, &UAfterImageComponent::SpawnDeferred, Time);
	}
	else
	{
		Spawn(AfterImage);
	}
}

void UAfterImageComponent::SpawnDeferred()
{
	for (FAfterImageStruct& elem : ImageData)
	{
		if (!elem.IsCreated)
		{
			elem.IsCreated = true;
			Spawn(elem);
			return;
		}
	}
}

void UAfterImageComponent::Spawn(const FAfterImageStruct & AfterImage)
{
	FTransform nT
	{
		FRotator(0.f, ((AfterImage.Rotation) ? 0.f : 180.f), 0.f),
		AfterImage.Location,
		FVector::OneVector
	};

	AAfterImage* nImage = GetWorld()->SpawnActorDeferred<AAfterImage>(ClassData.FindRef(AfterImage.ClassName), nT);
	if (nImage)
	{
		nImage->FinishSpawning(nT);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Error Spawning AfterImage - AfterImageComp"));
	}
}

void UAfterImageComponent::Empty()
{
	ImageData.Empty();
}