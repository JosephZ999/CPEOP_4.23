// Fill out your copyright notice in the Description page of Project Settings.

#include "Chars/AI/OgichiAI.h"
#include "Chars/Heroes/Ogichi.h"
#include "Kismet/KismetMathLibrary.h"

// Options
#define MIN_DIST_X 50.f
#define MIN_DIST_Y 20.f

#define MAX_DIST_X 100.f
#define MAX_DIST_Y 30.f

#define TP_CHANCE  0.3f

DEFINE_LOG_CATEGORY_STATIC(OgichiAI, All, All);

AOgichiAI::AOgichiAI() { SetSearchOptions(100.f, 10); }

void AOgichiAI::tpReload(float time)
{
	canTeleport = false;
	SET_TIMER(tpReloadTimer, this, &AOgichiAI::tpReset, time);
}

void AOgichiAI::tpReset() { canTeleport = true; }

void AOgichiAI::BeginPlay() { Super::BeginPlay(); }

void AOgichiAI::AIBody()
{
	UE_LOG(LogTemp, Warning, TEXT("AI Body"));

	if (! OgichiRef)
	{
		OgichiRef = Cast<AOgichi>(GetPawn());
		FTimerHandle IntroTimer;
		SET_TIMER(IntroTimer, this, &AOgichiAI::BreakIntro, 1.5f);
		return;
	}

	// Search Enemy
	if (! IsValid(getEnemy()))
	{
		if (! SearchEnemy(OgichiRef->GetTeam()))
		{
			if (GetAlly() && getDistance(GetAlly()->GetActorLocation()) > 100.f)
			{
				OgichiRef->SetMoveVector(getForwardVector(GetAlly()->GetActorLocation()));
			}
			else
			{
				OgichiRef->SetMoveVector(FVector::ZeroVector);
			}
			return;
		}
		else
		{
			EnemyInitialLocation = getEnemy()->GetActorLocation();
		}
	}
	else if (getEnemy()->IsDead())
	{
		OgichiRef->SetMoveVector(FVector::ZeroVector);
		OgichiRef->ResetKeys();
		ClearEnemy();
		return;
	}

	if (Intro)
	{
		if (! OgichiRef->GetHeroStats()->CheckSkill("Bankai"))
		{
			Intro = false;
			return;
		}

		if (EnemyInitialLocation.Equals(getEnemy()->GetActorLocation(), 10.f))
		{
			if (getEnemy()->IsImmortal())
			{
				OgichiRef->SkillEnable();
				OgichiRef->AttackDown();
				Intro = false;
				Wait(3.f);
			}
		}
		else
		{
			Intro = false;
			Cast<AHeroBase>(getEnemy())->GetHeroStats()->SetSkill("Bankai", false);
		}
		return;
	}

	switch (OgichiRef->GetState())
	{
	case EBaseStates::Stand:
	{
		A_Stand();
		return;
	}

	case EBaseStates::Blocking:
	{
		// if (getEnemy()->GetDangerType() == EDangerType::None) { OgichiRef->BlockStop(); }
		OgichiRef->BlockStop();
		Wait(0.1f);
		return;
	}

	case EBaseStates::Hit:
	{
		// Teleport
		if (canTeleport && UKismetMathLibrary::RandomBoolWithWeight(TP_CHANCE))
		{
			OgichiRef->SetMoveVector(getForwardVector());
			OgichiRef->Teleport();
			tpReload(0.5f);
			return;
		}
		Wait(0.1f);
		return;
	}

	case EBaseStates::Fall:
	{
		if (OgichiRef->GetVelocity().Z > 20.f) { OgichiRef->StandUp(); }
		return;
	}

	case EOgichiState::Ogi_Attack_1:
	{
		if (! OgichiRef->isComboTime()) { return; }

		if (getDistanceY() < MAX_DIST_Y)
		{
			// Next Attack
			if (getDistanceX() < 100.f)
			{
				OgichiRef->SetMoveVector((getDistanceX() < MIN_DIST_X) ? FVector::ZeroVector : getForwardVector());
				OgichiRef->SetRotation(isEnemyOnRight(), false);
				OgichiRef->Attack();
				Wait(0.5f);
				return;
			}
			else
			{
				// Teleport
				OgichiRef->SetMoveVector(getForwardVector());
				OgichiRef->Teleport();
				return;
			}
		}
		return;
	}

	case EOgichiState::Ogi_Attack_2:
	{
		if (! OgichiRef->isComboTime()) return;

		if (getDistanceY() > MAX_DIST_Y) return;

		// Sword Twist
		if (getDistanceX() > MAX_DIST_X)
		{
			OgichiRef->SetMoveVector((getDistanceX() < MIN_DIST_X) ? FVector::ZeroVector : getForwardVector());
			OgichiRef->SetRotation(isEnemyOnRight(), false);
			OgichiRef->AttackBack();
			Wait(0.5f);
			return;
		}
		else
		{
			if (FMath::RandBool())
			{
				// Attack Up
				OgichiRef->SetMoveVector((getDistanceX() < MIN_DIST_X) ? FVector::ZeroVector : getForwardVector());
				OgichiRef->SetRotation(isEnemyOnRight(), false);
				OgichiRef->AttackBack();
				Wait(0.5f);
				return;
			}
			else
			{
				// Attack Forward
				OgichiRef->SetMoveVector((getDistanceX() < MIN_DIST_X) ? FVector::ZeroVector : getForwardVector());
				OgichiRef->SetRotation(isEnemyOnRight(), false);
				OgichiRef->AttackForward();
				Wait(0.5f);
				return;
			}
		}
		return;
	}

	case EOgichiState::Ogi_SwordTwistLoop:
	{
		if (getEnemy()->IsFalling())
		{
			OgichiRef->Attack();
			return;
		}

		if (getDistanceY() > MAX_DIST_Y) return;

		// Ogi_Getsuga
		if (getDistanceX() > MAX_DIST_X && getDistanceX() < MAX_DIST_X * 4 && UKismetMathLibrary::RandomBoolWithWeight(0.2f) &&
			OgichiRef->GetHeroStats()->checkStamina(-(GETSUGA_TENSHOU_COST)) &&
			OgichiRef->GetHeroStats()->checkPower(-(GETSUGA_TENSHOU_COST)))
		{
			OgichiRef->SetRotation(isEnemyOnRight(), false);
			OgichiRef->SkillEnable();
			OgichiRef->AttackForward();
			return;
		}

		if (getDistanceX() > MAX_DIST_X && getDistanceX() < MAX_DIST_X * 2)
		{
			// Sword throw
			OgichiRef->SetMoveVector((getDistanceX() < MIN_DIST_X) ? FVector::ZeroVector : getForwardVector());
			OgichiRef->SetRotation(isEnemyOnRight(), false);
			OgichiRef->AttackForward();
			Wait(1.f);
			return;
		}
		else if (getDistanceX() < MAX_DIST_X)
		{
			// Attack Up
			OgichiRef->SetMoveVector((getDistanceX() < MIN_DIST_X) ? FVector::ZeroVector : getForwardVector());
			OgichiRef->SetRotation(isEnemyOnRight(), false);
			OgichiRef->AttackBack();
			Wait(1.f);
			return;
		}
		return;
	}

	} // End Switch
}

void AOgichiAI::A_Stand()
{
	CheckDanger();

	// Swort Twist if enemy fall
	if (getEnemy()->IsFalling())
	{
		OgichiRef->sh_SwordTwist();
		Wait(0.5f);
		return;
	}

	if (getDistanceX() < MAX_DIST_X && getDistanceY() < MAX_DIST_Y)
	{
		// Block
		/*
			if (getEnemy()->GetDangerType() == EDangerType::MeleeAttack &&
				((isEnemyOnRight() && ! getEnemy()->IsLookingRight()) || (! isEnemyOnRight() && getEnemy()->IsLookingRight())))
			{
				OgichiRef->SetMoveVector(getForwardVector());
				OgichiRef->Block();
				Wait(0.2f);
				return;
			}
		*/

		// Teleprot to behind of enemy
		/*
			if (UKismetMathLibrary::RandomBoolWithWeight(TP_CHANCE) && canTeleport)
			{
				OgichiRef->Teleport(getEnemyLocation() + ((isEnemyOnRight()) ? FVector(50.f, 0.f, 0.f) : FVector(-50.f, 0.f, 0.f)));
				tpReload(0.5f);
				return;
			}
		*/

		// Attack enemy
		OgichiRef->SetMoveVector((getDistanceX() < MIN_DIST_X) ? FVector::ZeroVector : getForwardVector());
		OgichiRef->SetRotation(isEnemyOnRight(), false);
		OgichiRef->Attack();
		Wait(0.25f);
		return;
	}

	// Move to enemy if he is away
	if (getDistanceX() > MAX_DIST_X || getDistanceY() > MIN_DIST_Y)
	{
		OgichiRef->SetMoveVector(getForwardVector(60));

		// Ogi_Getsuga
		if (UKismetMathLibrary::RandomBoolWithWeight(0.01f) && OgichiRef->GetHeroStats()->checkStamina(-(GETSUGA_COST)) &&
			OgichiRef->GetHeroStats()->checkPower(-(GETSUGA_COST)) && getDistanceY() < MIN_DIST_Y)
		{
			OgichiRef->SetRotation(isEnemyOnRight(), false);
			OgichiRef->SkillEnable();
			OgichiRef->AttackForward();
			return;
		}
	}
}

void AOgichiAI::CheckDanger()
{
	if (GetDangerInfo().Priority == EDangerPriority::None) return;
	if (! IsInsideADangerBox()) return;

	// Consts
	const float dangerLocationX	  = GetDangerInfo().DangerPosition.X;
	const float pawnLocationX	  = getPawnLocation().X;
	const bool	SetLookingToRight = dangerLocationX > pawnLocationX;

	// Action
	switch (GetDangerInfo().Priority)
	{
	case EDangerPriority::Normal:
	{
		A_Block(SetLookingToRight);
		ClearDangerInfo();
		return;

		if (UKismetMathLibrary::RandomBoolWithWeight(TP_CHANCE))
		{
			if (A_Teleport(GetDangerInfo().DangerPosition))
			{
				ClearDangerInfo();
				return;
			}
		}
		A_Block(SetLookingToRight);
		ClearDangerInfo();
		return;
	}
	case EDangerPriority::Ranged:
	{
		if (GetDangerInfo().Position.X > 0.f && GetDangerInfo().Position.X < 0.4f)
		{
			// Teleport
			if (A_Teleport(GetDangerInfo().DangerPosition))
			{
				ClearDangerInfo();
				return;
			}
		}
		A_Block(SetLookingToRight);
		ClearDangerInfo();
		return;
	}
	case EDangerPriority::Arrow:
	{
		if (GetDangerInfo().Position.X > 0.5f)
		{
			// Teleport
			if (A_Teleport(GetDangerInfo().DangerPosition))
			{
				ClearDangerInfo();
				return;
			}
			A_Block(SetLookingToRight);
		}
		ClearDangerInfo();
		return;
	}
	case EDangerPriority::Explosion:
	{
		FVector tp_Position = getPawnLocation();

		tp_Position.X = pawnLocationX + (SetLookingToRight) ? (-100.f) : 100.f;
		if (A_Teleport(tp_Position))
		{
			ClearDangerInfo();
			return;
		}
		A_Block(SetLookingToRight);
		ClearDangerInfo();
		return;
	}
	} // Switch End}
}

void AOgichiAI::A_Block(bool LookRight)
{
	const FVector MVector = {(LookRight ? 1.f : -1.f), 0.f, 0.f};
	OgichiRef->SetMoveVector(MVector);
	OgichiRef->Block();
	UE_LOG(LogTemp, Warning, TEXT("isBlocking"));
	Wait(0.2f);
}

bool AOgichiAI::A_Teleport(FVector Point)
{
	//
	const FVector MVector = UKismetMathLibrary::FindLookAtRotation(getPawnLocation(), Point).Vector();
	OgichiRef->SetMoveVector(MVector);
	return OgichiRef->Teleport();
}
