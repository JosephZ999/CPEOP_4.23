// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitStatsBase.h"
#include "HeroStats.generated.h"

/**
 *
 */

class AMyPlayerController;

USTRUCT()
struct FFormStats
{
	GENERATED_BODY()

	FFormStats(uint8 inStr, uint8 inAgi, uint8 inSpi)
		: strength(inStr)
		, agility(inAgi)
		, spirit(inSpi)
	{
	}

	FFormStats() {}

	uint8 strength{0};
	uint8 agility{0};
	uint8 spirit{0};

	bool isValid() { return (strength || agility || spirit); }
};

USTRUCT(BlueprintType)
struct FSavedStats
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	uint8 level{1};
	UPROPERTY(BlueprintReadWrite)
	uint8 strength{0};
	UPROPERTY(BlueprintReadWrite)
	uint8 agility{0};
	UPROPERTY(BlueprintReadWrite)
	uint8 spirit{0};

	UPROPERTY(BlueprintReadWrite)
	int32 maxExp{0};
	UPROPERTY(BlueprintReadWrite)
	int32 exp{0};

	UPROPERTY(BlueprintReadWrite)
	TMap<FName, bool> Skills;

	void addStr(uint8 num) { strength += num; }
	void addAgi(uint8 num) { agility += num; }
	void addSpi(uint8 num) { spirit += num; }
};

UCLASS()
class CPEOP_API UHeroStats : public UUnitStatsBase
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	FName FormName;

private:
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess = "true"))
	TMap<FName, FFormStats> HeroForms;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	FSavedStats SavedStats;

	// Base Stats
	uint8 Strength;	 //
	uint8 Agility;	 //
	uint8 Spirit;	 //
	int32 MaxExp{0}; //
	int32 Exp{0};	 //

	// Strength
	float MaxHealth; //
	float Health;	 //
	float Damage;	 //
	float Armor;	 //

	// Agility
	float AttackSpeed;	//
	float WalkSpeed;	//
	float SprintSpeed;	//
	float CritRate;		//
	float TeleportCost; //

	// Intelligence
	float MaxPower;			   // ����. �������� �������
	float Power;			   // ����� �������� �������
	float Stamina;			   // �������� �������
	float SkillReducer;		   // ��������� ������� �������
	float StaminaRestoreSpeed; //

	float ExpMultiplier{1.f};

public:
	virtual void SetLevel(uint8 NewLevel) override;

	virtual float GetHealth() const override;
	virtual float GetMaxHealth() const override;
	virtual float GetDamage() const override;
	virtual float GetCritRate() const override;

	void AddForms(FName name, FVector stats);

	UFUNCTION(BlueprintCallable)
	void SetForm(FName name);

	UFUNCTION(BlueprintCallable)
	void SetExpMultiplier(float value) { ExpMultiplier = value; }

	UFUNCTION(BlueprintCallable)
	uint8 GetLevel() const { return Level; }

	UFUNCTION(BlueprintCallable)
	float GetPower() const { return Power; }

	UFUNCTION(BlueprintCallable)
	float GetMaxPower() const { return MaxPower; }

	UFUNCTION(BlueprintCallable)
	int32 GetExp() const { return Exp; }

	UFUNCTION(BlueprintCallable)
	int32 GetMaxExp() const { return MaxExp; }

	UFUNCTION(BlueprintCallable)
	int32 GetLevelScore() const;

	UFUNCTION(BlueprintCallable)
	void GetAllStats(float& Dmg, float& Arm, float& Crit, float& Speed, float& TpCos, float& SkillReduce)
	{
		Dmg			= Damage;
		Arm			= Armor;
		Crit		= CritRate;
		Speed		= WalkSpeed;
		TpCos		= TeleportCost;
		SkillReduce = SkillReducer;
	}

	UFUNCTION(BlueprintCallable)
	float GetStamina() const { return Stamina; }

	bool CheckPower(float power, float stamina = 0.f, bool skill = false) const;

	float GetWalkSpeed() { return WalkSpeed; }
	float GetSprintSpeed() { return SprintSpeed; }
	float GetTeleportCost() { return TeleportCost; }

	// Set
public:
	virtual void Init() override;

	void LevelUp();

	UFUNCTION(BlueprintCallable)
	bool AddStats(FVector stats, bool force = true);

	void AddHealth(float value);
	void SetHealth(float value);
	void AddPower(float value);
	void SetPower(float value);
	void AddExp(int32 value);
	void SetExp(int32 value);
	void Leveling();

	UFUNCTION(BlueprintCallable)
	void SetSkill(FName skillName, bool value);

	UFUNCTION(BlueprintCallable)
	bool CheckSkill(FName key);

	AMyPlayerController* GetOwnerController() const;

	virtual float TakeDamage(float damage, float armorPiercing, bool blocked) override;

	//---------------------------------------------// Stamina
public:
	virtual void  AddStamina(float value, float time = 0.f, bool skill = false, int desiredState = -1) override;
	virtual void  RestoreStamina() override;
	virtual bool  RestoreStaminaCondition() override { return Stamina < 1.f; }
	virtual float RestoreStaminaDelay() override { return 1.f; }
};
