// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sys/Interfaces/GModeInterface.h"
#include "Objects/Dynamic/PickUpBase.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

class AUnitBase;
class AHeroBase;
class AMonsterBase;
class AMyPlayerController;
class APickUpBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartBattle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFinishBattle, EGameResultType, Results);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitDead, AUnitBase*, Unit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeroLevelUp, AHeroBase*, Hero);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWavePassed);

USTRUCT(BlueprintType)
struct FSpawnParams
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FTransform Transform;

	UPROPERTY(BlueprintReadWrite)
	uint8 Team;

	UPROPERTY(BlueprintReadWrite)
	uint8 Level = 1;
};

USTRUCT(BlueprintType)
struct FWaveMonster
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<AMonsterBase> MonsterClass;

	UPROPERTY(BlueprintReadWrite)
	int32 Number;

	UPROPERTY(BlueprintReadWrite)
	uint8 Level;

	UPROPERTY(BlueprintReadWrite)
	uint8 LevelAdd;
};

/**
 *
 */
UCLASS()
class CPEOP_API AMyGameModeBase : public AGameModeBase, public IGModeInterface
{
	GENERATED_BODY()

private:
	UPROPERTY()
	AHeroBase*			 _PlayerHero;
	AMyPlayerController* _PlayerController;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> _HeroSpawnEffect;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> _MonsterSpawnEffect;

	// Game Timer
	int32			 _GameTime = 0;
	FTimerHandle	 _GameTimer;
	FORCEINLINE void GameTimeSec() { ++_GameTime; }

	// Player Kills
	int32			 _PlayerKills = 0;
	FORCEINLINE void AddKill() { ++_PlayerKills; }

	// Monsters Waves
	TArray<FWaveMonster> _CurrentWave;
	int32				 _WavesPassed;
	int32				 _MonstersInLevel;
	int32				 _MaxMonstersInLevel;
	int32				 _MonsterIndex;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE AMyPlayerController* GetController() const { return _PlayerController; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AHeroBase* GetPlayerHero() const { return _PlayerHero; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetGameTime() const { return _GameTime; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetPlayerKills() const { return _PlayerKills; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetWavesNum() const { return _WavesPassed; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetMonstersNum() const { return _MonstersInLevel; }

	// Delegates
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "My Delegates")
	FOnStartBattle OnStartBattle;

	UPROPERTY(BlueprintAssignable, Category = "My Delegates")
	FOnFinishBattle OnFinishBattle;

	UPROPERTY(BlueprintAssignable, Category = "My Delegates")
	FOnUnitDead OnUnitDead;

	UPROPERTY(BlueprintAssignable, Category = "My Delegates")
	FOnHeroLevelUp OnHeroLevelUp;

	UPROPERTY(BlueprintAssignable, Category = "My Delegates")
	FOnWavePassed OnWavePassed;

public:
	//------------------------------------------// Functional
	UFUNCTION(BlueprintCallable)
	AHeroBase* SpawnAHero(TSubclassOf<AHeroBase> HeroClass, FSpawnParams Params, bool Possess = false, bool Effect = false);

	UFUNCTION(BlueprintCallable)
	bool PossessToHero(AHeroBase* Hero);

	UFUNCTION(BlueprintCallable)
	AMonsterBase* SpawnAMonster(TSubclassOf<AMonsterBase> MonsterClass, FSpawnParams Params, bool Effect = true);

	UFUNCTION(BlueprintCallable)
	int32 SpawnAWave(TArray<FWaveMonster> Monsters, int32 MaxInLevel, bool Shuffle = true);
	void  SpawnNextDeferred(float Delay);
	void  SpawnNext();

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
