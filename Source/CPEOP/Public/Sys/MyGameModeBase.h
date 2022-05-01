// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GModeInterface.h"
#include "PickUpBase.h"
#include "BCoreTypes.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

class AMyPlayerController;

/**
 *
 */
UCLASS()
class CPEOP_API AMyGameModeBase : public AGameModeBase, public IGModeInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "My Delegates")
	FOnStartBattleSignature OnStartBattle;

	UPROPERTY(BlueprintAssignable, Category = "My Delegates")
	FOnFinishBattleSignature OnFinishBattle;

	UPROPERTY(BlueprintAssignable, Category = "My Delegates")
	FOnUnitDeadSignature OnUnitDead;

	UPROPERTY(BlueprintAssignable, Category = "My Delegates")
	FOnHeroLevelUpSignature OnHeroLevelUp;

	UPROPERTY(BlueprintAssignable, Category = "My Delegates")
	FOnWavePassedSignature OnWavePassed;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBGameData _GameData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameSettings _GameSettings;

private:
	UPROPERTY()
	AHeroBase* _PlayerHero;

	UPROPERTY()
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

	bool _GamePaused = false;

public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

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
	void SpawnPickUp(AHeroBase* OwnerHero, FPickUpParams Params);

	// Events
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartGame();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FinishGame(const FText& Title, EGameResultType GameResult);

	// Getters
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

	//------------------------------------------// Game Mode Interface
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GM Interface")
	void SetGamePaused();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GM Interface")
	void LevelUp(AActor* Hero);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GM Interface")
	void Kill(AUnitBase* Killer, AUnitBase* Killed);
};
