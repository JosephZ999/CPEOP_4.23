// Fill out your copyright notice in the Description page of Project Settings.

#include "Sys/MyGameModeBase.h"
#include "Sys/Interfaces/AIEvents.h"
#include "Sys/MyPlayerController.h"
#include "Engine/World.h"

#include "Chars/HeroBase.h"
#include "Chars/MonsterBase.h"

#define PLAYER_INDEX 0

AHeroBase* AMyGameModeBase::SpawnAHero_Implementation(
	TSubclassOf<AHeroBase> HeroClass, FTransform SpawnTransform, uint8 Team, uint8 Level, bool Possess)
{
	AHeroBase* Hero = GetWorld()->SpawnActorDeferred<AHeroBase>(HeroClass, SpawnTransform);
	if (Hero)
	{
		Hero->SetTeam(Team);
		Hero->GetHeroStats()->SetLevel(Level);
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

AMonsterBase* AMyGameModeBase::SpawnAMonster_Implementation(
	TSubclassOf<AMonsterBase> MonsterClass, FTransform SpawnTransform, uint8 Team, uint8 Level)
{
	AMonsterBase* Monster = GetWorld()->SpawnActorDeferred<AMonsterBase>(MonsterClass, SpawnTransform);
	if (Monster)
	{
		Monster->SetTeam(Team);
		Monster->GetUnitStats()->SetLevel(Level);
		Monster->FinishSpawning(SpawnTransform);
		return Monster;
	}
	return nullptr;
}

void AMyGameModeBase::SpawnPickUp(TSubclassOf<APickUpBase> Class, AHeroBase* OwnerHero, FPickUpParams Params)
{
	while (Params.Amount > 0)
	{
		if (FMath::RandRange(0.f, 100.f) > Params.Chance)
		{
			return;
		}

		FTransform	 nT(FRotator::ZeroRotator, Params.Location, FVector::OneVector);
		APickUpBase* nObject = GetWorld()->SpawnActorDeferred<APickUpBase>(Class, nT);
		if (nObject)
		{
			nObject->Init(Params.Level, OwnerHero);
			nObject->FinishSpawning(nT);
		}
		--Params.Amount;
	}
}

//----------------------------------------------// Events
void AMyGameModeBase::StartGame_Implementation() {}

void AMyGameModeBase::FinishGame_Implementation(const FText& Title, EGameResultType GameResult)
{
	OnFinishBattle.Broadcast(GameResult);
}

//----------------------------------------------// Game Mode Interface
void AMyGameModeBase::LevelUp_Implementation(AActor* Hero)
{
	AHeroBase* HeroRef = Cast<AHeroBase>(Hero);
	OnHeroLevelUp.Broadcast(HeroRef);
}

void AMyGameModeBase::Kill_Implementation(AActor* Killer, AActor* Killed)
{
	AUnitBase* UnitRef = Cast<AUnitBase>(Killed);
	OnUnitDead.Broadcast(UnitRef);
}
