// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class CPEOP_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay();

public:

	//==========================================/ GAME EVENTS
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Game Events")
	void EventKill(class AUnitBase * killerUnit, class AUnitBase * killedUnit);
};
