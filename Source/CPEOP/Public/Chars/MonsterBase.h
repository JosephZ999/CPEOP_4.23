// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Chars/Components/MonsterStats.h"

#include "CoreMinimal.h"
#include "Chars/UnitBase.h"
#include "MonsterBase.generated.h"

/**
 * 
 */

UENUM()
enum class EMonsterStates : uint8
{
	Attack_1 = (uint8)EBaseStates::LastIndex,
	Attack_2,
	AttackRanged_1,
	AttackRanged_2,
	Super,
};

UCLASS()
class CPEOP_API AMonsterBase : public AUnitBase
{
	GENERATED_BODY()

public:
	AMonsterBase();

private:
	/** Характеристики персонажа */
	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	class UMonsterStats* Stats;

	UPROPERTY(EditDefaultsOnly, Category = "Monster Options")
	float DeathEffScale { 1.f };

public:
	FORCEINLINE virtual class UUnitStatsBase* getStatsComp() const { return Stats; }
	FORCEINLINE class UMonsterStats* getStats()              const { return Stats; }

protected:
	virtual void BeginPlay() override;
	virtual void OnDamaged() override;

private:
	void Appearance();
	void Death();


//---------------------------------------------// Attack / Combo Time
public:
	virtual void Attack();
	FORCEINLINE bool ComboTime() { return IsAttacking; }
protected:
	void AttackDuration(float Duration);
private:
	void AttackSuccess();
	FTimerHandle AttackEndTimer;
	bool IsAttacking;

};
