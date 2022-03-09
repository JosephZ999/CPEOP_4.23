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
	// Get
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Unit Interface", Meta = (Keywords = "Get"))
	uint8 GetUnitTeam();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Unit Interface", Meta = (Keywords = "Get"))
	int32 GetExpForKill();

	// Components
	/*
		UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Unit Interface", Meta = (Keywords = "Get"))
		class UUnitStats* GetUnitStats();

		UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Unit Interface", Meta = (Keywords = "Get"))
		class UHeroStats* GetHeroStats();

		UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Unit Interface", Meta = (Keywords = "Get"))
		class UMonsterStats* GetMonsterStats();
	*/

	// State
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Unit Interface", Meta = (Keywords = "IsA"))
	bool IsAlive();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Unit Interface", Meta = (Keywords = "IsH"))
	bool IsItHero();

	// Add Stats ...
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Unit Interface", Meta = (Keywords = "Add"))
	bool AddHealth(float Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Unit Interface", Meta = (Keywords = "Add"))
	bool AddPower(float Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Unit Interface", Meta = (Keywords = "Add"))
	bool AddExp(int32 Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Unit Interface", Meta = (Keywords = "Add"))
	void UseLevelPoints();

	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Unit Getters")
	// void GetUnitDamage();

	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Unit Getters")
	// void GetUnitCritRate();
};
