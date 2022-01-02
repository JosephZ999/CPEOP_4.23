// Fill out your copyright notice in the Description page of Project Settings.


#include "Sys/MyGameInstance.h"
#include "Paper2D/Classes/PaperFlipbook.h"


	UPaperFlipbook* UMyGameInstance::getSpark(uint8 index) const
	{
		if (Data_Sparks.Num() > index)
		{
			return Data_Sparks[index];
		}
		return nullptr;
	}

	TSubclassOf<class ADmgTextBase> UMyGameInstance::getDamageTextClass() const
	{
		return DamageText;
	}

	bool UMyGameInstance::canCreateDamageText(bool crited) const
	{
		if (crited && ShowCritDamageText)
		{
			return true;
		}
		else
		{
			return FMath::RandRange(0, 100) <= DamageTextRate;
		}
	}