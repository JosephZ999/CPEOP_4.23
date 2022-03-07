// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AIEvents.generated.h"

UENUM(BlueprintType)
enum class EDangerPriority : uint8
{
	None,
	Normal,
	Ranged,
	Arrow,
	Explosion,
};

USTRUCT(BlueprintType)
struct FDangerArg
{
	GENERATED_BODY()

	FDangerArg()
	// : param1
	// , param2
	{
	}
	EDangerPriority Priority;

	/* Value (0-1)
	 * Position X - if (Priority == Normal) returns 0 if a Character is near Enemy
	 * Position X - if (Priority == Explosion) returns 0 when a Character in center
	 * Position Y -> 0 when Character in center (0-1)
	 */
	FVector2D Position;

	//
	FVector2D StartLocation;
	FVector2D EndLocation;

	FVector DangerPosition;
};

USTRUCT(BlueprintType)
struct FDangerOptions
{
	GENERATED_BODY()

	FDangerOptions() {}

	FDangerOptions(FVector nScale)
		: Location(FVector(nScale.X * 25.f, 0.f, 0.f))
		, Scale(nScale)
		, LifeTime(0.3f)
		, AttachToOwner(true)
		, ShowRadius(true)
	{
	}

	FDangerOptions(FVector nLocation, FVector nScale)
		: Location(nLocation)
		, Scale(nScale)
		, LifeTime(0.3f)
		, AttachToOwner(true)
		, ShowRadius(true)
	{
	}

	FVector Location;
	FVector Scale;
	float	LifeTime;
	bool	AttachToOwner;
	bool	ShowRadius;
};

USTRUCT(BlueprintType)
struct FAIOptions
{
	GENERATED_BODY()

	FAIOptions() {}

	UPROPERTY(BlueprintReadWrite) float MinDist	  = 50.f;
	UPROPERTY(BlueprintReadWrite) float MaxDist	  = 100.f;
	UPROPERTY(BlueprintReadWrite) float AttackVel = 1.f;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAIEvents : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class CPEOP_API IAIEvents
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI Interface")
	void SetAIEnabled(bool Enable);

	UFUNCTION(BlueprintNativeEvent, Category = "AI Interface")
	void OnDangerDetected(FDangerArg& DangerInfo);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI Interface")
	void SetEnemy(class AUnitBase* ObjectRef);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI Interface")
	void SetAIOptions(const FAIOptions& Options);

	/*

	void GetEnemy();
	void SetEnemy();
	...

	*/
};
