// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroInput.h"
#include "TimerManager.h"

// Add default functionality here for any IHeroInput functions that are not pure virtual.

void IHeroInput::BtnDashReset()
{
	BtnDashVector = FVector::ZeroVector;
}