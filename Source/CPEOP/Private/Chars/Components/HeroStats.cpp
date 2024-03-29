// Fill out your copyright notice in the Description page of Project Settings.


#include "Chars/Components/HeroStats.h"
#include "Sys/MyPlayerController.h"

#include "TimerManager.h"
#include "Engine/World.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Consts per stat
#define HEALTH           2.f
#define	DAMAGE           0.5f
#define	ARMOR            0.1F

#define WALK_SPEED       5.f
#define RUN_SPEED        20.f
#define CRIT_RATE        0.5f
#define TP_COST          0.5f // �� ������� �������� ����� ������� ��� �����������������

#define POWER            2.f
#define SKILL_REDUCER    0.25f
#define STAMINA_REGEN    0.01f // Restor duration 10 second

#define BASE_WALK_SPEED  150.f
#define BASE_HEALTH      10.F
#define BASE_POWER       10.F

void UHeroStats::BeginPlay()
{
	Super::BeginPlay();
}

void UHeroStats::Init()
{
	FFormStats form = HeroForms.FindRef(FormName);
	
	if (!form.isValid())
	{
		UE_LOG(LogTemp, Fatal, TEXT("Initialization stats error, HeroFormName: %s"), *FormName.ToString());
		return;
	}

	bool firstTime = (Level == 0);

	Level		= SavedStats.level;
	Strength	= form.strength + SavedStats.strength;
	Agility		= form.agility	+ SavedStats.agility;
	Spirit		= form.spirit	+ SavedStats.spirit;
	
	if (!MaxExp)
	{
		MaxExp	= Level * 100;
		Exp		= SavedStats.exp;
	}

	// Strength
	if (firstTime)
	{
		MaxHealth	= Strength * HEALTH;
		Health		= MaxHealth;
	}
	else 
	{ 
		float healthPercent = Health / MaxHealth;
		MaxHealth = Strength * HEALTH;
		Health = MaxHealth * healthPercent;
	}

	Damage		= Strength * DAMAGE;
	Armor		= Strength * ARMOR;

	// Agility
	// AttackSpeed		= Agility * Consts.AttackSpeed_PerAgi;
	WalkSpeed		= Agility * WALK_SPEED + BASE_WALK_SPEED;
	SprintSpeed		= Agility * RUN_SPEED + BASE_WALK_SPEED;
	TeleportCost	= 1.f + Agility * TP_COST;

	// Intelligence
	if (firstTime)
	{
		MaxPower = Spirit * POWER;
		Power = MaxPower;
		Stamina = 1.f;
	}
	else
	{
		float powerPercent = Power / MaxPower;
		MaxPower = Spirit * POWER;
		Power = MaxPower * powerPercent;
	}
	SkillReducer	= 1.f + Spirit * SKILL_REDUCER;
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
	Exp += exp;

	if (Exp < MaxExp)
		return;

	FTimerHandle levelingTimer;
	while (Exp > MaxExp)
	{
		++Level;
		Exp		-= MaxExp;
		MaxExp	 = Level * 100;

		// Saving changes
		SavedStats.level	= Level;
		SavedStats.exp		= Exp;
		SavedStats.maxExp	= MaxExp;

		// ����� ������ �������� �����������
		GetWorld()->GetTimerManager().SetTimer(levelingTimer, this, &UHeroStats::LevelUp, 0.2f);
	}
}

void UHeroStats::LevelUp()
{
	AMyPlayerController* controller = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (controller)
	{
		controller->CharLevelUp(this);
	}
	else UE_LOG(LogTemp, Fatal, TEXT("Couldn't find PlayerController"));
}

void UHeroStats::AddStats(FVector stats)
{
	SavedStats.addStr(stats.X);
	SavedStats.addAgi(stats.Y);
	SavedStats.addSpi(stats.Z);
	Init();
}

float UHeroStats::TakeDamage(float damage, bool blocked)
{
	if (blocked)
		damage = FMath::Max(0.f, damage - Armor * 5);
	else
		damage = FMath::Max(0.f, damage - Armor);

	Health -= damage;
	return damage;
}


//---------------------------------------------// Stamina
	void UHeroStats::AddStamina(float value, float time, bool skill)
	{
		Super::AddStamina(value, time, skill);

		if (time > 0.f)
			return;

		if (skill)
		{
			Stamina = FMath::Clamp(Stamina + value / SkillReducer, 0.f, 2.f);
			Power = FMath::Clamp(Power + value, 0.f, MaxPower);
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
