// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sys/Interfaces/GModeInterface.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

class AHeroBase;
class AMyPlayerController;

/**
 *
 */

UCLASS()
class CPEOP_API AMyGameModeBase : public AGameModeBase, public IGModeInterface
{
	GENERATED_BODY()

private:
	AHeroBase*			 PlayerHero;
	AMyPlayerController* PlayerController;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE AMyPlayerController* GetController() const { return PlayerController; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AHeroBase* GetPlayerHero() const { return PlayerHero; }

public:
	//------------------------------------------// Game Mode Interface
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AHeroBase* SpawnAHero(TSubclassOf<AHeroBase> HeroClass, FTransform SpawnTransform, uint8 Team, bool Possess);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool PossessToHero(AHeroBase* Hero);
};
