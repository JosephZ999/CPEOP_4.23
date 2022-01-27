// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Chars/Components/MonsterStats.h"

#include "CoreMinimal.h"
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

UCLASS()
class CPEOP_API AMonsterBase : public AUnitBase
{
	GENERATED_BODY()

public:
	AMonsterBase();

private:
	/** �������������� ��������� */
	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UMonsterStats* Stats;

	UPROPERTY(EditDefaultsOnly, Category = "UnitOptions|Data")
	TMap<FName, UPaperFlipbook*> AnimList;
	
	UPROPERTY(EditDefaultsOnly, Category = "Monster Options")
	float DeathEffScale { 1.f };

	UPROPERTY(EditDefaultsOnly, Category = "Monster Options")
	float SpawnEffScale{ 1.f };


public:
	FORCEINLINE virtual UUnitStatsBase* getStatsComp() const { return Stats; }
	FORCEINLINE UMonsterStats* getStats()              const { return Stats; }

	void AddAnimation(FName index, UPaperFlipbook* elem = nullptr);
	UPaperFlipbook* GetAnimation(FName index);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetSpawnEffScale() { return SpawnEffScale; }

protected:
	virtual void BeginPlay() override;
	virtual void OnDamaged() override;

private:
	void Appearance();
	void Death();


//---------------------------------------------// Attack / Combo Time
public:
	virtual void Attack();
	FORCEINLINE bool ComboTime() { return !IsAttacking; }
protected:
	void AttackDuration(float Duration);
private:
	void AttackSuccess();
	FTimerHandle AttackEndTimer;
	bool IsAttacking;

};
