// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UnitGetters.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUnitGetters : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class CPEOP_API IUnitGetters
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Unit Getters")
	void GetUnitTeam();

	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Unit Getters")
	// void GetUnitDamage();

	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Unit Getters")
	// void GetUnitCritRate();
};
