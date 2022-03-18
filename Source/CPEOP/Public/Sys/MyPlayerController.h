// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "sys/Interfaces/PlayerHUD.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 *
 */

UCLASS()
class CPEOP_API AMyPlayerController : public APlayerController, public IPlayerHUD
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ShowInputWidget();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void HideInputWidget();

	// Character Calls
	UFUNCTION(BlueprintImplementableEvent)
	void HeroSkillActivated();

	UFUNCTION(BlueprintImplementableEvent)
	void HeroSkillDeactivated();
};
