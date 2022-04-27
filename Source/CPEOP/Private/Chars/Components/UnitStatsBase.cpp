// Fill out your copyright notice in the Description page of Project Settings.

#include "UnitStatsBase.h"
#include "UnitBase.h"
#include "TimerManager.h"
#include "Engine/World.h"

#define STAMINA_RESTORE_TIME 0.1f

UUnitStatsBase::UUnitStatsBase()
{
	PrimaryComponentTick.bCanEverTick = false;

	AddStaminaValue = {0};
}

void UUnitStatsBase::SetLevel(uint8 NewLevel)
{
	Level = FMath::Clamp((int32)NewLevel, 1, 100);
	Init();
}

void UUnitStatsBase::BeginPlay()
{
	Super::BeginPlay();

	OwnerState = &(Cast<AUnitBase>(GetOwner())->GetStateRef());
}

void UUnitStatsBase::AddStamina(float value, float time, bool skill, int desiredState)
{
	if (time > 0.f)
	{
		AddStaminaValue = value;
		AddStaminaSkill = skill;

		if (desiredState >= 0)
			DesiredState = desiredState;

		GetWorld()->GetTimerManager().SetTimer(AddStaminaTimer, this, &UUnitStatsBase::AddStaminaDeffered, time);
	}
	else
	{
		// Restore
		GetWorld()->GetTimerManager().SetTimer(RestoreStaminaTimer, this, &UUnitStatsBase::RestoreStamina, RestoreStaminaDelay());
	}
}

void UUnitStatsBase::AddStaminaDeffered()
{
	AddStamina(AddStaminaValue, -1.f, AddStaminaSkill, DesiredState);
}

void UUnitStatsBase::RestoreStamina()
{
	if (RestoreStaminaCondition())
	{
		GetWorld()->GetTimerManager().SetTimer(RestoreStaminaTimer, this, &UUnitStatsBase::RestoreStamina, STAMINA_RESTORE_TIME);
	}
}

bool UUnitStatsBase::AddStaminaCanceled()
{
	return ! (*OwnerState == DesiredState);
}
