// Fill out your copyright notice in the Description page of Project Settings.

#include "Shrieker.h"
#include "ShriekerSummon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

AShrieker::AShrieker()
{
	AddAnimation(FName("Stand"), nullptr);
	AddAnimation(FName("Walk"), nullptr);
	AddAnimation(FName("Attack"), nullptr);
	AddAnimation(FName("Hit"), nullptr);

	InitHelper("AttackSlash");

	CanFall = false;

	SummonsNum = 0;
}

//--------------------------------------//

void AShrieker::BeginPlay()
{
	Super::BeginPlay();
}

// Animation

void AShrieker::AnimUpdate()
{
	if (CheckState(EBaseStates::Stand))
	{
		if (MoveVector.IsNearlyZero(0.1f) || FMath::IsNearlyZero(GetUnitVelocity().X, 1.f))
		{
			SetAnim(FName("Stand"), false);
		}
		else
		{
			SetAnim(FName("Walk"), false);
		}
	}
}

void AShrieker::EndState()
{
	Super::EndState();

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

	FState nState;

	switch (GetState())
	{
	case EBaseStates::Jumping:
	{
		NewState(nState);
		break;
	}
	case EBaseStates::JumpLand:
	{
		NewState(nState);
		break;
	}
	} // Switch End
}

void AShrieker::Landed(const FHitResult& Hit)
{
	if (CheckState(EBaseStates::Hit))
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	}
}

//--------------------------------------//

void AShrieker::Attack()
{
	FState nState;
	nState.State	 = EMonsterStates::M_Attack_1;
	nState.Animation = "Attack";
	NewState(nState);

	SpawnHelperDeferred("AttackSlash", AnimElemTime(3));
	AddImpulse(MoveVector * 200.f, AnimElemTime(2));
}

void AShrieker::Summon()
{
	FState nState;
	nState.State	 = EMonsterStates::M_Super;
	nState.Animation = "Attack";
	NewState(nState);

	BP_Summon();
}

AShriekerSummon* AShrieker::SpawnSummon(TSubclassOf<class AShriekerSummon> Class, FVector Location)
{
	if (SummonsNum > 0)
	{
		FTransform		 nTransform{FRotator(0.f), Location, FVector(1.f)};
		AShriekerSummon* nSummon = GetWorld()->SpawnActorDeferred<AShriekerSummon>(Class, nTransform);
		if (nSummon)
		{
			UGameplayStatics::FinishSpawningActor(nSummon, nTransform);
			--SummonsNum;
			return nSummon;
		}
	}
	return nullptr;
}

void AShrieker::SetSummonsNum(uint8 Num)
{
	SummonsNum = Num;
}
