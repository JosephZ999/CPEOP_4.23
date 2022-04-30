// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BCoreTypes.h"
#include "GModeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGModeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class CPEOP_API IGModeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GM Interface")
	void SetGamePaused();

	// Stats
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GM Interface") FGameResults GetGameResults();

	// Events
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GM Interface")
	void Kill(AUnitBase* Killer, AUnitBase* Killed);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GM Interface")
	void LevelUp(AActor* Hero);

	// Conditions
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GM Interface")
	bool CanShakeCamera(AActor* OwnerUnit, AActor* DamagedUnit);
};
