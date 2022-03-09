// Fill out your copyright notice in the Description page of Project Settings.

#include "Chars/Components/HeroStats.h"
#include "Sys/MyPlayerController.h"

#include "TimerManager.h"
#include "Engine/World.h"

#include "Sys/Interfaces/GameIns.h"

// Consts per stat
#define HEALTH			2.f
#define DAMAGE			0.5f
#define ARMOR			0.1F

#define WALK_SPEED		5.f
#define RUN_SPEED		20.f
#define CRIT_RATE		0.5f
#define TP_COST			0.5f // За единицу ловкости можно столько раз телепортироваться

#define POWER			2.f
#define SKILL_REDUCER	0.25f
#define STAMINA_REGEN	0.01f // Restor duration 10 second

#define BASE_WALK_SPEED 150.f
#define BASE_HEALTH		10.F
#define BASE_POWER		10.F

#define HP_RESTORE		0.25f // Percent
#define MP_RESTORE		0.25f // Percent

void UHeroStats::BeginPlay()
{
	Super::BeginPlay();
}

float UHeroStats::GetHealth() const
{
	return Health;
}

float UHeroStats::GetMaxHealth() const
{
	return FMath::Max(0.1f, MaxHealth);
}

float UHeroStats::GetDamage() const
{
	return FMath::Max(0.1f, Damage);
}

float UHeroStats::GetCritRate() const
{
	return CritRate;
}

void UHeroStats::Init()
{
	FFormStats form = HeroForms.FindRef(FormName);

	if (! form.isValid())
	{
		UE_LOG(LogTemp, Fatal, TEXT("Initialization stats error, HeroFormName: %s"), *FormName.ToString());
		return;
	}

	bool firstTime = (Level == 0);

	Level	 = SavedStats.level;
	Strength = form.strength + SavedStats.strength;
	Agility	 = form.agility + SavedStats.agility;
	Spirit	 = form.spirit + SavedStats.spirit;

	if (! MaxExp)
	{
		MaxExp = Level * 100;
		Exp	   = SavedStats.exp;
	}

	// Strength
	if (firstTime)
	{
		MaxHealth = Strength * HEALTH;
		Health	  = MaxHealth;
	}
	else
	{
		float healthPercent = Health / MaxHealth;
		MaxHealth			= Strength * HEALTH;
		Health				= MaxHealth * healthPercent;
	}

	Damage = Strength * DAMAGE;
	Armor  = Strength * ARMOR;

	// Agility
	// AttackSpeed		= Agility * Consts.AttackSpeed_PerAgi;
	WalkSpeed	 = Agility * WALK_SPEED + BASE_WALK_SPEED;
	SprintSpeed	 = Agility * RUN_SPEED + BASE_WALK_SPEED;
	TeleportCost = 1.f + Agility * TP_COST;
	CritRate	 = 1.f + Agility * CRIT_RATE;

	// Intelligence
	if (firstTime)
	{
		MaxPower = Spirit * POWER;
		Power	 = MaxPower;
		Stamina	 = 1.f;
	}
	else
	{
		float powerPercent = Power / MaxPower;
		MaxPower		   = Spirit * POWER;
		Power			   = MaxPower * powerPercent;
	}
	SkillReducer		= 1.f + Spirit * SKILL_REDUCER;
	StaminaRestoreSpeed = STAMINA_REGEN;
}

// Transformation //=============================------------------------------
void UHeroStats::AddForms(FName name, FVector stats)
{
	if (FormName == "None")
		FormName = name;

	HeroForms.Add(name, FFormStats(stats.X, stats.Y, stats.Z));
}

void UHeroStats::SetForm(FName name)
{
	if (HeroForms.FindRef(name).isValid())
	{
		FormName = name;
		Init();
	}
}

// End Transformation //=============================------------------------------

void UHeroStats::AddExp(int32 exp)
{
	exp = (float)exp * ExpMultiplier;
	Exp += exp;

	if (Exp < MaxExp)
		return;

	FTimerHandle levelingTimer;
	while (Exp >= MaxExp)
	{
		++Level;
		Exp -= MaxExp;
		MaxExp = Level * 100;

		// Saving changes
		SavedStats.level  = Level;
		SavedStats.exp	  = Exp;
		SavedStats.maxExp = MaxExp;

		GetWorld()->GetTimerManager().SetTimer(levelingTimer, this, &UHeroStats::LevelUp, 0.2f);
	}
}

void UHeroStats::LevelUp()
{
	AddHealth(GetMaxHealth() * HP_RESTORE);
	AddPower(GetMaxPower() * MP_RESTORE);
	IGameIns::Execute_OnHeroLevelUp(GetWorld()->GetGameInstance(), GetOwner());
}

bool UHeroStats::AddStats(FVector stats, bool force)
{
	int32 nStats = stats.X + stats.Y + stats.Z;
	if (GetLevelScore() >= nStats || force)
	{
		SavedStats.addStr(stats.X);
		SavedStats.addAgi(stats.Y);
		SavedStats.addSpi(stats.Z);
		Init();
		return true;
	}
	return false;
}

int32 UHeroStats::GetLevelScore() const
{
	return SavedStats.level * 2 - (SavedStats.strength + SavedStats.agility + SavedStats.spirit + 2);
}

float UHeroStats::TakeDamage(float damage, float armorPiercing, bool blocked)
{
	if (blocked)
		damage = FMath::Max(0.f, damage - Armor * 10 * (1.f - armorPiercing));
	else
		damage = FMath::Max(0.f, damage - Armor * (1.f - armorPiercing));

	Health -= damage;
	return damage;
}

//---------------------------------------------// Stamina
void UHeroStats::AddStamina(float value, float time, bool skill, int desiredState)
{
	Super::AddStamina(value, time, skill, desiredState);

	if (time > 0.f)
		return;

	if (desiredState >= 0 && AddStaminaCanceled())
		return;

	if (skill)
	{
		Stamina = FMath::Clamp(Stamina + value / SkillReducer, 0.f, 2.f);
		Power	= FMath::Clamp(Power + value, 0.f, MaxPower);
		return;
	}
	else
	{
		Stamina = FMath::Clamp(Stamina + value, 0.f, 2.f);
	}
}

void UHeroStats::RestoreStamina()
{
	Super::RestoreStamina();

	if (RestoreStaminaCondition())
	{
		Stamina = FMath::Min(Stamina + StaminaRestoreSpeed, 1.f);
	}
}

bool UHeroStats::checkStamina(float value, bool skill) const
{
	if (skill)
	{
		return (value / SkillReducer) < Stamina;
	}
	else
	{
		return value <= Stamina;
	}
}

bool UHeroStats::checkPower(float value) const
{
	return value <= Power;
}

void UHeroStats::AddHealth(float value)
{
	Health = FMath::Clamp(Health + value, 0.1f, MaxHealth);
}

void UHeroStats::AddPower(float value)
{
	Power = FMath::Clamp(Power + value, 0.1f, MaxPower);
}

void UHeroStats::SetSkill(FName skillName, bool value)
{
	if (SavedStats.Skills.Contains(skillName))
	{
		SavedStats.Skills[skillName] = value;
	}
	else
	{
		SavedStats.Skills.Add(skillName, value);
	}
}

bool UHeroStats::CheckSkill(FName key)
{
	if (SavedStats.Skills.Contains(key))
	{
		return SavedStats.Skills[key];
	}
	else
	{
		return false;
	}
}
