// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameIns.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGameIns : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class CPEOP_API IGameIns
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "GameIns Interface")
	void PauseGame();
	//------------------------------------------// Events

	UFUNCTION(BlueprintNativeEvent, Category = "GameIns Events")
	void OnUnitKilled(class AUnitBase* Killer, class AUnitBase* KilledUnit);

	UFUNCTION(BlueprintNativeEvent, Category = "GameIns Events")
	void OnHeroLevelUp(AActor* TargetActor);

	//------------------------------------------// Getters

	UFUNCTION(BlueprintNativeEvent, Category = "GameIns Events")
	class UPaperFlipbook* GetSparkAnimation(uint8 index);

	UFUNCTION(BlueprintNativeEvent, Category = "GameIns Events")
	class TSubclassOf<class AHelper> GetDamageTextClass();

	UFUNCTION(BlueprintNativeEvent, Category = "GameIns Events")
	bool CanCreateDamageText(bool Crited);

	UFUNCTION(BlueprintNativeEvent, Category = "GameIns Events")
	TSubclassOf<class ADangerBox> GetDangerBoxClass();
};
