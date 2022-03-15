// Fill out your copyright notice in the Description page of Project Settings.

#include "Sys/MyGameModeBase.h"
#include "Sys/Interfaces/AIEvents.h"
#include "Sys/MyPlayerController.h"
#include "Engine/World.h"
#include "Chars/HeroBase.h"

#define PLAYER_INDEX 0

//----------------------------------------------// Game Mode Interface

AHeroBase* AMyGameModeBase::SpawnAHero_Implementation(TSubclassOf<AHeroBase> HeroClass, FTransform SpawnTransform, uint8 Team, bool Possess)
{
	AHeroBase* Hero = GetWorld()->SpawnActorDeferred<AHeroBase>(HeroClass, SpawnTransform);
	if (Hero)
	{
		Hero->SetTeam(Team);
		Hero->FinishSpawning(SpawnTransform);
		if (Possess)
		{
			PossessToHero(Hero);
		}
		return Hero;
	}

	return nullptr;
}

bool AMyGameModeBase::PossessToHero_Implementation(AHeroBase* Hero)
{
	if (IsValid(Hero) && Hero->GetClass()->ImplementsInterface(UAIEvents::StaticClass()))
	{
		IAIEvents::Execute_SetAIEnabled(Hero, false);

		UWorld* World = Hero->GetWorld();
		uint32	Index = 0;
		for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			APlayerController* nController = Iterator->Get();
			if (Index == PLAYER_INDEX)
			{
				nController->Possess(Hero);
				PlayerHero		 = Hero;
				PlayerController = Cast<AMyPlayerController>(nController);
				return true;
			}
			Index++;
		}
	}
	return false;
}
