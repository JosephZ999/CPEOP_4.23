// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UnitInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUnitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class CPEOP_API IUnitInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Unit Interface", Meta = (Keywords = "Team"))
	uint8 GetUnitTeam();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Unit Interface", Meta = (Keywords = "Hero"))
	bool IsItHero();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Unit Interface", Meta = (Keywords = "Alive"))
	bool IsAlive();

	// Add Stats ...
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Unit Interface", Meta = (Keywords = "Add"))
	bool AddHealth(float Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Unit Interface", Meta = (Keywords = "Add"))
	bool AddPower(float Value);

	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Unit Getters")
	// void GetUnitDamage();

	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Unit Getters")
	// void GetUnitCritRate();
};
