// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameModeBase.h"
#include "AIEvents.h"
#include "PlayerHUD.h"
#include "MyPlayerController.h"
#include "MyFunctionLibrary.h"
#include "TimerManager.h"
#include "Engine/World.h"

#include "HeroBase.h"
#include "MonsterBase.h"

#include "Engine.h"

#define PLAYER_INDEX 0

//----------------------------------------------// Functional
AHeroBase* AMyGameModeBase::SpawnAHero(TSubclassOf<AHeroBase> HeroClass, FSpawnParams Params, bool Possess, bool Effect)
{
	AHeroBase* Hero = GetWorld()->SpawnActorDeferred<AHeroBase>(HeroClass, Params.Transform);
	if (Hero)
	{
		Hero->SetTeam(Params.Team);
		Hero->GetHeroStats()->SetLevel(Params.Level);
		Hero->FinishSpawning(Params.Transform);

		if (Effect && _HeroSpawnEffect)
		{
			FVector EffLocation = Hero->GetActorLocation();
			EffLocation.Y += 25.f;
			EffLocation.Z = 0.f;
			Params.Transform.SetLocation(EffLocation);
			Params.Transform.SetScale3D(Hero->GetActorScale3D());
			GetWorld()->SpawnActor<AActor>(_MonsterSpawnEffect, Params.Transform);
		}

		if (Possess)
		{
			PossessToHero(Hero);
		}
		return Hero;
	}
	return nullptr;
}

bool AMyGameModeBase::PossessToHero(AHeroBase* Hero)
{
	if (! IsValid(Hero))
	{
		return false;
	}

	UWorld* World = Hero->GetWorld();
	uint32	Index = 0;
	for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* nController = Iterator->Get();
		if (Index == PLAYER_INDEX)
		{
			IAIEvents::Execute_SetAIEnabled(Hero, false);
			nController->Possess(Hero);

			if (IsValid(_PlayerHero))
			{
				IAIEvents::Execute_SetAIEnabled(_PlayerHero, true);
			}
			_PlayerHero = Hero;

			if (! _PlayerController)
			{
				_PlayerController = Cast<AMyPlayerController>(nController);
			}
			return true;
		}
		Index++;
	}
	return false;
}

AMonsterBase* AMyGameModeBase::SpawnAMonster(TSubclassOf<AMonsterBase> MonsterClass, FSpawnParams Params, bool Effect)
{
	AMonsterBase* Monster = GetWorld()->SpawnActor<AMonsterBase>(MonsterClass, Params.Transform);
	if (Monster)
	{
		Monster->SetTeam(Params.Team);
		Monster->GetUnitStats()->SetLevel(Params.Level);
		++_MonstersInLevel;

		if (Effect && _MonsterSpawnEffect)
		{
			FVector EffLocation = Monster->GetActorLocation();
			EffLocation.Y += 25.f;
			EffLocation.Z = 0.f;
			Params.Transform.SetLocation(EffLocation);
			Params.Transform.SetScale3D(Monster->GetActorScale3D());
			GetWorld()->SpawnActor<AActor>(_MonsterSpawnEffect, Params.Transform);
		}
		return Monster;
	}
	return nullptr;
}

int32 AMyGameModeBase::SpawnAWave(TArray<FWaveMonster> Monsters, int32 MaxInLevel, bool Shuffle)
{
	GEngine->AddOnScreenDebugMessage(0, 10.f, //
		FColor::Yellow,						  //
		FString(" Spawn Wave"),				  //
		true, FVector2D(2.f, 2.f));

	_CurrentWave.Empty();
	_MaxMonstersInLevel = MaxInLevel;
	_MonsterIndex		= 0;

	for (const FWaveMonster& Element : Monsters)
	{
		if (! Element.MonsterClass)
		{
			continue;
		}

		for (int32 i = Element.Number; i > 0; i--)
		{
			_CurrentWave.Add(Element);
		}
	}

	if (Shuffle)
	{
		for (int32 i = 0; i < _CurrentWave.Num(); i++)
		{
			_CurrentWave.Swap(i, FMath::RandRange(0, _CurrentWave.Num() - 1));
		}
	}

	int32 SpawnNum = _MaxMonstersInLevel - _MonstersInLevel;
	while (SpawnNum > 0)
	{
		SpawnNextDeferred(1.f);
		--SpawnNum;
	}
	return _CurrentWave.Num();
}

void AMyGameModeBase::SpawnNextDeferred(float Delay)
{
	Delay = FMath::FRandRange(0.2f, Delay);
	FTimerHandle nTimer;
	GetWorldTimerManager().SetTimer(nTimer, this, &AMyGameModeBase::SpawnNext, Delay);
}

void AMyGameModeBase::SpawnNext()
{
	if (_CurrentWave.Num() == 0)
		return;

	if (_MonsterIndex < _CurrentWave.Num() && _MonstersInLevel < _MaxMonstersInLevel)
	{
		FWaveMonster MonsterInfo = _CurrentWave[_MonsterIndex];

		const FQuat Rotation(FRotator(0.f, ((FMath::RandBool()) ? 0.f : 180.f), 0.f));

		FSpawnParams nParams;
		nParams.Level = MonsterInfo.Level + FMath::RandRange(0, MonsterInfo.LevelAdd);
		nParams.Team  = 10;

		nParams.Transform.SetScale3D(FVector::OneVector);
		nParams.Transform.SetLocation(UMyFunctionLibrary::FindRandomLocation(_PlayerHero, 100.f, 300.f));
		nParams.Transform.SetRotation(Rotation);

		AMonsterBase* Monster = SpawnAMonster(MonsterInfo.MonsterClass, nParams);

		if (Monster)
		{
			++_MonsterIndex;
		}
	}
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
void AMyGameModeBase::StartGame_Implementation()
{
	GetWorldTimerManager().SetTimer(_GameTimer, this, &AMyGameModeBase::GameTimeSec, 1.f, true);
	OnStartBattle.Broadcast();
}

void AMyGameModeBase::FinishGame_Implementation(const FText& Title, EGameResultType GameResult)
{
	GetWorldTimerManager().PauseTimer(_GameTimer);
	OnFinishBattle.Broadcast(GameResult);
}

//----------------------------------------------// Game Mode Interface
void AMyGameModeBase::LevelUp_Implementation(AActor* Hero)
{
	AHeroBase* HeroRef = Cast<AHeroBase>(Hero);
	if (HeroRef == _PlayerHero)
	{
		IPlayerHUD::Execute_LevelUpdated(_PlayerController, HeroRef->GetHeroStats()->GetLevel());
		IPlayerHUD::Execute_ShowWarningNotice(_PlayerController, false);
	}

	OnHeroLevelUp.Broadcast(HeroRef);
}

void AMyGameModeBase::Kill_Implementation(AUnitBase* Killer, AUnitBase* Killed)
{
	if (Killer == _PlayerHero)
	{
		AddKill();
	}

	if (! IUnitInterface::Execute_IsItHero(Killed))
	{
		--_MonstersInLevel;
		SpawnNextDeferred(1.f);

		if (_MonstersInLevel == 0)
		{
			GEngine->AddOnScreenDebugMessage(0, 10.f, //
				FColor::White,						  //
				FString("Monsters = 0"),			  //
				true, FVector2D(2.f, 2.f));

			if (_MonsterIndex >= _CurrentWave.Num() - 1)
			{
				GEngine->AddOnScreenDebugMessage(0, 10.f, //
					FColor::White,						  //
					FString("NextWave"),				  //
					true, FVector2D(2.f, 2.f));

				++_WavesPassed;
				if (_PlayerController)
				{
					IPlayerHUD::Execute_ShowWaveScore(_PlayerController, _WavesPassed, _PlayerKills);
				}
				OnWavePassed.Broadcast();
			}
		}
	}

	// Exp
	Killer->GetUnitStats()->AddExp(Killed->GetUnitStats()->GetExpForKill());

	OnUnitDead.Broadcast(Killed);
}
