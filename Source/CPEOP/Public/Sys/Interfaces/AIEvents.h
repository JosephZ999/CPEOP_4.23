// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AIEvents.generated.h"

USTRUCT(BlueprintType)
struct FDangerArg
{
	GENERATED_BODY()

		FDangerArg()
		// : param1
		// , param2
	{}
	FVector2D Size;
	FVector2D Position; // 0.5 - Actor in center of danger region
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
	UFUNCTION(BlueprintNativeEvent, Category = "AI Interface")
	void OnDangerDetected(FDangerArg& Arg1);

	/*
	
	void GetEnemy();
	void SetEnemy();
	...

	*/
};