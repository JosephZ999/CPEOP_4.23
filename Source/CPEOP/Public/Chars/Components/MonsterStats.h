// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chars/Components/UnitStatsBase.h"
#include "MonsterStats.generated.h"

/**
 * 
 */
UCLASS()
class CPEOP_API UMonsterStats : public UUnitStatsBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void Init(uint8 level);

private:
	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = 1, ClampMax = 100))
	uint8 Level { 1 };

	float Health;
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth;
	UPROPERTY(EditDefaultsOnly)
	float Damage;
	UPROPERTY(EditDefaultsOnly)
	float CritRate;

public:
	virtual float GetHealth()     const override { return Health;    }
	virtual float GetMaxHealth()  const override { return MaxHealth; }
	virtual float GetDamage()     const override { return Damage;    }
	virtual float GetCritRate()   const override { return CritRate;  }

	virtual float TakeDamage(float damage, bool block) override;

	//

	uint8 GetLevel() { return Level; }


};
