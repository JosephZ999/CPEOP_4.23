// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum EGameResultType
{
	Win, Greate, Perfect, Lose,
};

USTRUCT(BlueprintType)
struct FGameResults
{
	GENERATED_BODY()

	FGameResults() {}

	UPROPERTY(BlueprintReadWrite)
	FText Title;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EGameResultType> Result { EGameResultType::Win };

	UPROPERTY(BlueprintReadWrite)
	int32 Time { 0 };

	UPROPERTY(BlueprintReadWrite)
	int32 Kills { 0 };
};

UCLASS()
class CPEOP_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay();


public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintPure)
	FGameResults GetGameResults();

	//==========================================/ GAME EVENTS
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Game Events")
	void EventKill(class AUnitBase * killerUnit, class AUnitBase * killedUnit);
};
