// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimerManager.h"
#include "Objects/Dynamic/AfterImage.h"
#include "Components/ActorComponent.h"
#include "AfterImageComponent.generated.h"

class UPaperFlipbook;

USTRUCT()
struct FAfterImageStruct
{
	GENERATED_BODY()

	FAfterImageStruct() {}

	FAfterImageStruct(FName nClass, FVector nLoc, bool nRot, int8 nFrame)
		: ClassName(nClass)
		, Location(nLoc)
		, Rotation(nRot)
		, Frame(nFrame)
	{
	}

	FAfterImageStruct(FVector nLoc, bool nRot, UPaperFlipbook* nAnim, int8 nFrame)
		: Location(nLoc)
		, Rotation(nRot)
		, Animation(nAnim)
		, Frame(nFrame)
	{
	}

	FName			ClassName;
	FVector			Location{0.f, 0.f, 0.f};
	bool			Rotation{false};
	UPaperFlipbook* Animation{nullptr};
	int8			Frame{0};
	bool			LoopAnim{false};
	bool			OneFrame{true};
	bool			IsCreated{false};

	inline void SetDistance(float Dist, const FRotator& Rotator) { Location += Rotator.RotateVector(FVector(Dist, 0.f, 0.f)); }
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CPEOP_API UAfterImageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAfterImageComponent();

	UFUNCTION()
	void InitClass();

	UFUNCTION()
	void Create(FAfterImageStruct& AfterImage, float Time);

	UFUNCTION()
	void Empty();

private:
	UFUNCTION()
	void Spawn(const FAfterImageStruct& AfterImage);

	UFUNCTION()
	void SpawnDeferred();

	TArray<FAfterImageStruct> ImageData;

public:
	UPROPERTY(EditDefaultsOnly, Category = "UnitData")
	TMap<FName, TSubclassOf<AAfterImage>> ClassData;
};
