// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */

UCLASS()
class CPEOP_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ShowInputWidget();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void HideInputWidget();

	// Character Calls
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CharLevelUp();

	UFUNCTION(BlueprintImplementableEvent)
	void HeroSkillActivated();

	UFUNCTION(BlueprintImplementableEvent)
	void HeroSkillDeactivated();

};
