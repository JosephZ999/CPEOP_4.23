// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sys/Interfaces/GModeInterface.h"
#include "Objects/Dynamic/PickUpBase.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartBattle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFinishBattle, EGameResultType, Results);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitDead, class AUnitBase*, Unit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeroLevelUp, class AHeroBase*, Hero);

// DECLARE_DYNAMIC_DELEGATE(FOnStartBattle);

class AUnitBase;
class AHeroBase;
class AMonsterBase;
class AMyPlayerController;
class APickUpBase;

/**
 *
 */
UCLASS()
class CPEOP_API AMyGameModeBase : public AGameModeBase, public IGModeInterface
{
	GENERATED_BODY()

private:
	AHeroBase*			 _PlayerHero;
	AMyPlayerController* _PlayerController;

	// Game Timer
	int32		 _GameTime = 0;
	FTimerHandle _GameTimer;
	inline void	 GameTimeSec() { ++_GameTime; }

	// Player Kills
	int32		_PlayerKills = 0;
	inline void AddKill() { ++_PlayerKills; }

public:
	UFUNCTION(BlueprintCallable) FORCEINLINE AMyPlayerController* GetController() const { return _PlayerController; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AHeroBase* GetPlayerHero() const { return _PlayerHero; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetGameTime() const { return _GameTime; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetPlayerKills() const { return _PlayerKills; }

	// Delegates
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "My Delegates")
	FOnStartBattle OnStartBattle;

	UPROPERTY(BlueprintAssignable, Category = "My Delegates")
	FOnFinishBattle OnFinishBattle;

	UPROPERTY(BlueprintAssignable, Category = "My Delegates")
	FOnUnitDead OnUnitDead;

	UPROPERTY(BlueprintAssignable, Category = "My Delegates")
	FOnHeroLevelUp OnHeroLevelUp;

public:
	//------------------------------------------// Functional
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AHeroBase* SpawnAHero(TSubclassOf<AHeroBase> HeroClass, FTransform SpawnTransform, uint8 Team, uint8 Level = 1, bool Possess = false);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AMonsterBase* SpawnAMonster(TSubclassOf<AMonsterBase> MonsterClass, FTransform SpawnTransform, uint8 Team, uint8 Level = 1);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool PossessToHero(AHeroBase* Hero);

	UFUNCTION(BlueprintCallable)
	void SpawnPickUp(TSubclassOf<APickUpBase> Class, AHeroBase* OwnerHero, FPickUpParams Params);

	// Events
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartGame();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FinishGame(const FText& Title, EGameResultType GameResult);

	//------------------------------------------// Game Mode Interface
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GM Interface")
	void LevelUp(AActor* Hero);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GM Interface")
	void Kill(AUnitBase* Killer, AUnitBase* Killed);
};
