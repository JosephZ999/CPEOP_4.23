// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chars/Components/MonsterStats.h"
#include "Chars/UnitBase.h"
#include "MonsterBase.generated.h"

/**
 *
 */

class UPaperFlipbook;
class UMonsterStats;

UENUM()
enum EMonsterStates
{
	M_Attack_1 = EBaseStates::LastIndex,
	M_Attack_2,
	M_AttackRanged_1,
	M_AttackRanged_2,
	M_Super,
};

UCLASS(Abstract)
class CPEOP_API AMonsterBase : public AUnitBase
{
	GENERATED_BODY()

public:
	AMonsterBase();

private:
	/** Характеристики персонажа */
	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UMonsterStats* Stats;

	UPROPERTY(EditDefaultsOnly, Category = "UnitOptions|Data")
	TMap<FName, UPaperFlipbook*> AnimList;

	UPROPERTY(EditDefaultsOnly, Category = "Monster Options")
	float DeathEffScale{1.f};

	UPROPERTY(EditDefaultsOnly, Category = "Monster Options")
	float SpawnEffScale{1.f};

public:
	FORCEINLINE virtual UUnitStatsBase* GetUnitStats() const { return Stats; }
	FORCEINLINE UMonsterStats* getStats() const { return Stats; }

	bool IsOnGround();

	void			AddAnimation(FName index, UPaperFlipbook* elem = nullptr);
	UPaperFlipbook* GetAnimation(FName index);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetSpawnEffScale() { return SpawnEffScale; }

protected:
	virtual void BeginPlay() override;

private:
	void Appearance();
	void Death();

	// Animation
protected:
	virtual void AnimUpdate() {}

private:
	FTimerHandle AnimUpdateTimer;

	UPROPERTY(EditDefaultsOnly)
	float AnimUpdateRate{0.03f};

	//---------------------------------------------// Attack / Combo Time
public:
	virtual void	 Attack();
	FORCEINLINE bool ComboTime() { return ! IsAttacking; }

	virtual void EventDead() override;

protected:
	void AttackDuration(float Duration);

private:
	void		 AttackSuccess();
	FTimerHandle AttackEndTimer;
	bool		 IsAttacking;

	//------------------------------------------// Unit Interface
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Unit Interface", Meta = (Keywords = "Hero"))
	bool IsItHero();
	//------------------------------------------// AI Interface

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI Interface")
	void SetAIEnabled(bool Enable);
};
