// Fill out your copyright notice in the Description page of Project Settings.

#include "Sys/MyGameModeBase.h"
#include "Sys/Interfaces/AIEvents.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "Chars/HeroBase.h"

#define PLAYER_INDEX 0

AHeroBase* AMyGameModeBase::GetPlayerHero_Implementation()
{
	if (IsValid(PlayerHero))
	{
		return PlayerHero;
	}
	return nullptr;
}

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
			APlayerController* PlayerController = Iterator->Get();
			if (Index == PLAYER_INDEX)
			{
				PlayerController->Possess(Hero);
				PlayerHero = Hero;
				return true;
			}
			Index++;
		}
	}
	return false;
}
