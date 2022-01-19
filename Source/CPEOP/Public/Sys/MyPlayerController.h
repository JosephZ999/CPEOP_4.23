// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */

class AHeroBase;

UENUM(BlueprintType)
enum EInputActionType
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

UCLASS()
class CPEOP_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	AHeroBase* PlayerCharacter = nullptr;

	UFUNCTION(BlueprintCallable)
	void SetPlayerHero(AHeroBase* Hero) { PlayerCharacter = Hero; }

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ShowInputWidget();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void HideInputWidget();

	// Character Calls
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CharLevelUp(UHeroStats* StatsComp);

	UFUNCTION(BlueprintImplementableEvent)
	void HeroSkillActivated();

	UFUNCTION(BlueprintImplementableEvent)
	void HeroSkillDeactivated();

protected:
	virtual void BeginPlay();


public:
	// Inputs
	UFUNCTION(BlueprintCallable, Category = Input)
	void BtnAction(enum EInputActionType action, bool btnReleased);

	UFUNCTION(BlueprintCallable, Category = Input)
	void BtnDash(FVector forwardVector, bool Released);
	FTimerHandle DashTimer;
	FVector DashVector;
	void DashDisable();


};
