// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AIEvents.generated.h"

UENUM(BlueprintType)
enum class EDangerPriority : uint8
{
	Normal,
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
	FVector2D Size;
	FVector2D Position; // 0.5 - Actor in center of danger region
};

USTRUCT(BlueprintType)
struct FDangerOptions
{
	GENERATED_BODY()

	FDangerOptions() {}

	FDangerOptions(FVector nScale)
		: Location(FVector::ZeroVector)
		, Scale(nScale)
		, LifeTime(0.3f)
		, AttachToOwner(true)
		, ShowRadius(true)
	{
	}

	FDangerOptions(FVector nScale, FVector nLocation)
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

	UPROPERTY(BlueprintReadWrite)
	float MinDist;
	UPROPERTY(BlueprintReadWrite)
	float MaxDist;
	UPROPERTY(BlueprintReadWrite)
	float AttackVel;
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
	void OnDangerDetected(FDangerArg& Arg1, EDangerPriority Arg2);

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
