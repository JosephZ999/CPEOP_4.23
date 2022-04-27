// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerHUD.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerHUD : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class CPEOP_API IPlayerHUD
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowGameUI(bool Hide);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void LevelUpdated(uint8 Level);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HpUpdated(float Hp, float MaxHp);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void MpUpdated(float Mp, float MaxMp);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ExpUpdated(float Exp, float MaxExp);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StaminaUpdated(float Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowInputUI(bool Hide);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowWarningNotice(bool Hide);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowWaveScore(int32 WaveNum, int32 TotalKills);

	//-------------------------------------------

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SkillActivated();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SkillDeactivated();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void NotEnoughPower();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void NotEnoughStamina();
};
