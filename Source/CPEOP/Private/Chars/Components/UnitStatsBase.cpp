// Fill out your copyright notice in the Description page of Project Settings.


#include "Chars/Components/UnitStatsBase.h"
#include "Engine/World.h"
#include "TimerManager.h"

#include "Chars/UnitBase.h"

#define STAMINA_RESTORE_TIME 0.1F

// Sets default values for this component's properties
UUnitStatsBase::UUnitStatsBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	AddStaminaValue = { 0 };
	// ...
}


// Called when the game starts
void UUnitStatsBase::BeginPlay()
{
	Super::BeginPlay();

	// ...


}


// Called every frame
void UUnitStatsBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UUnitStatsBase::AddStamina(float value, float time, bool skill)
{
	if (time > 0.f)
	{
		// Delay
		AddStaminaValue = value;
		AddStaminaSkill = skill;
		GetWorld()->GetTimerManager().SetTimer(AddStaminaTimer, this, &UUnitStatsBase::AddStaminaDeffered, time);
	}
	else
	{
		// Restore
		GetWorld()->GetTimerManager().SetTimer(RestoreStaminaTimer, this, &UUnitStatsBase::RestoreStamina, RestoreStaminaDelay());
	}

	// ...
}

void UUnitStatsBase::AddStaminaDeffered()
{
	AddStamina(AddStaminaValue, -1.f, AddStaminaSkill);
}

void UUnitStatsBase::RestoreStamina()
{
	if (RestoreStaminaCondition())
	{
		GetWorld()->GetTimerManager().SetTimer(RestoreStaminaTimer, this, &UUnitStatsBase::RestoreStamina, STAMINA_RESTORE_TIME);
	}
}