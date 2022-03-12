// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sys/Interfaces/GModeInterface.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

class AHeroBase;

/**
 *
 */

UCLASS()
class CPEOP_API AMyGameModeBase : public AGameModeBase, public IGModeInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "Game Mode")
	AHeroBase* PlayerHero;

public:
	//------------------------------------------// Game Mode Interface
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GM Interface")
	AHeroBase* SpawnAHero(TSubclassOf<AHeroBase> HeroClass, FTransform SpawnTransform, uint8 Team, bool Possess);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GM Interface")
	bool PossessToHero(AHeroBase* Hero);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GM Interface")
	AHeroBase* GetPlayerHero();
};
