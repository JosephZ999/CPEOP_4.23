// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class CPEOP_API UMyFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	static int RotationDivide(FVector2D pointA, FVector2D pointB);
	UFUNCTION(BlueprintCallable)
	static int RotationDivide2(FVector2D CurrPoint, FVector2D CenterPoint, uint8 divisionNum, float& angle);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static float PercentDivision(float percent, uint8 divisionNum);
};
