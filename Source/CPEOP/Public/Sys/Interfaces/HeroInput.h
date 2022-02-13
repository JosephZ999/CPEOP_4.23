// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HeroInput.generated.h"

UENUM(BlueprintType)
enum class EInputActionType : uint8
{
	A_None,
	A_Center,
	A_Up,
	A_Down,
	A_Forward,
	A_Backward,

	A_BlockStart,
	A_BlockEnd,
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHeroInput : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CPEOP_API IHeroInput
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

// Control Type 1
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Hero Commands")
	void BtnSetMovement(FVector Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Hero Commands")
	void BtnAction(EInputActionType action, bool btnReleased);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Hero Commands")
	void BtnDash(FVector forwardVector, bool Released);

	FVector BtnDashVector;
	FTimerHandle BtnDashTimer;
	void BtnDashReset();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Hero Commands")
	void BtnTeleport();

};
