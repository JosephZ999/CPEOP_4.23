// Fill out your copyright notice in the Description page of Project Settings.

#include "Sys/MyGameInstance.h"
#include "Paper2D/Classes/PaperFlipbook.h"

// Interface Functions

UPaperFlipbook* UMyGameInstance::GetSparkAnimation_Implementation(uint8 index)
{
	if (FightClasses.Sparks.Num() > index) { return FightClasses.Sparks[index]; }
	return nullptr;
}

TSubclassOf<class AHelper> UMyGameInstance::GetDamageTextClass_Implementation() { return FightClasses.DamageText; }

bool UMyGameInstance::CanCreateDamageText_Implementation(bool Crited)
{
	if (Crited && ShowCritDamageText) { return true; }
	else
	{
		return FMath::RandRange(1, 100) <= DamageTextRate;
	}
}

TSubclassOf<class ADangerBox> UMyGameInstance::GetDangerBoxClass_Implementation() { return FightClasses.DangerBox; }
