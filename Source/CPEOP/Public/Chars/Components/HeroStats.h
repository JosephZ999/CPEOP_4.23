// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chars/Components/UnitStatsBase.h"
#include "HeroStats.generated.h"

/**
 * 
 */

USTRUCT()
struct FFormStats
{
	GENERATED_BODY()

	FFormStats(uint8 inStr, uint8 inAgi, uint8 inSpi) 
		: strength(inStr)
		, agility(inAgi)
		, spirit(inSpi)
	{}

	FFormStats(){}

	uint8 strength	{ 0 };
	uint8 agility	{ 0 };
	uint8 spirit	{ 0 };

	bool isValid() { return (strength || agility || spirit); }
};

USTRUCT(BlueprintType)
struct FSavedStats
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	uint8 level		{ 1 };
	UPROPERTY(BlueprintReadWrite)
	uint8 strength	{ 0 };
	UPROPERTY(BlueprintReadWrite)
	uint8 agility	{ 0 };
	UPROPERTY(BlueprintReadWrite)
	uint8 spirit	{ 0 };

	UPROPERTY(BlueprintReadWrite)
	int32 maxExp	{ 0 };
	UPROPERTY(BlueprintReadWrite)
	int32 exp		{ 0 };

	void addStr(uint8 num) { strength += num;	}
	void addAgi(uint8 num) { agility += num;	}
	void addSpi(uint8 num) { spirit += num;	}

};

UCLASS()
class CPEOP_API UHeroStats : public UUnitStatsBase
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
	FName FormName;

private:
// Base Stats
	int8 Level { 0 };
	int8 Strength;		int8 Agility;		int8 Spirit;
	int32 MaxExp{ 0 };	int32 Exp{ 0 };

// Strength
	float MaxHealth;	float Health;
	float Damage;		float Armor;
	
// Agility
	float AttackSpeed;	float WalkSpeed;	float SprintSpeed;
	float CritRate;		float TeleportCost;
	
// Intelligence
	float MaxPower;		// Макс. духовная энергия
	float Power;		// Запас духовной энергии
	float Stamina;		// Активная энергия
	float SkillReducer; // Уменьшает затраты навыков
	float StaminaRestoreSpeed;

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess = "true"))
	TMap<FName, FFormStats> HeroForms;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	FSavedStats SavedStats;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	/** Добавляет форму для персонажа как Банкай
	* Вызывается в конструкторе персонажа */
	void AddForms(FName name, FVector stats);
	UFUNCTION(BlueprintCallable)
	void SetForm(FName name);

	
	virtual float GetHealth()const		override { return Health; }
	virtual float GetMaxHealth()const	override { return MaxHealth; }
	virtual float GetDamage()const		override { return Damage; }
	virtual float GetCritRate()const	override { return CritRate; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float getPower()const			{ return Power; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float getMaxPower()const		{ return MaxPower; }

	UFUNCTION(BlueprintCallable)
	void getStats(uint8& Lv, uint8& Str, uint8& Agi, uint8& Spi, float& Dmg, float& Arm, float& Crit)const
	{
		Lv	= Level;
		Str = Strength;
		Agi = Agility;
		Spi = Spirit;
		Dmg = Damage;
		Arm = Armor;
		Crit = CritRate;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float getStamina()const			{ return Stamina; }

	bool checkStamina(float value, bool skill = true)const;
	bool checkPower(float value)const;


	FORCEINLINE float getWalkSpeed()			{ return WalkSpeed;		}
	FORCEINLINE float getSprintSpeed()			{ return SprintSpeed;	}

	FORCEINLINE float getTeleportCost()			{ return TeleportCost; }
public:
	void Init();

	void AddExp(int32 exp);
	void LevelUp();

	UFUNCTION(BlueprintCallable)
	void AddStats(FVector stats);

	virtual float TakeDamage(float damage, bool blocked) override;

//---------------------------------------------// Stamina
public:
	virtual void     AddStamina(float value, float time = 0.f, bool skill = false) override;
	virtual void     RestoreStamina()          override;
	virtual bool     RestoreStaminaCondition() override { return Stamina < 1.f;  }
	virtual float    RestoreStaminaDelay()     override { return 1.f; }
};
