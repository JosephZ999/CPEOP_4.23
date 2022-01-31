// Fill out your copyright notice in the Description page of Project Settings.

#include "Chars/AI/OgichiAI.h"
#include "Chars/Heroes/Ogichi.h"
#include "Kismet/KismetMathLibrary.h"

// Options
#define MIN_DIST_X 50.f
#define MIN_DIST_Y 20.f

#define MAX_DIST_X 100.f
#define MAX_DIST_Y 30.f

#define TP_CHANCE 0.3f

AOgichiAI::AOgichiAI()
{
	SearchStepRadius = 50.f;
	SearchSteps = 15;
}

void AOgichiAI::tpReload(float time)
{
	canTeleport = false;
	SET_TIMER(tpReloadTimer, this, &AOgichiAI::tpReset, time);
}

void AOgichiAI::tpReset()
{
	canTeleport = true;
}

void AOgichiAI::BeginPlay()
{
	Super::BeginPlay();
}

void AOgichiAI::AIBody()
{
	if (!OgichiRef)
	{
		OgichiRef = Cast<AOgichi>(GetPawn());
		FTimerHandle IntroTimer;
		SET_TIMER(IntroTimer, this, &AOgichiAI::BreakIntro, 1.5f);
		return;
	}

	// Search Enemy
	if (!getEnemy())
	{
		if (!SearchEnemy(OgichiRef->GetTeam()))
		{
			Wait(1.f);
			return;
		}
		else
		{
			EnemyInitialLocation = getEnemy()->GetActorLocation();
		}
	}

	if (Intro)
	{
		if (!OgichiRef->getHeroStatsComp()->CheckSkill("Bankai"))
		{
			Intro = false;
			return;
		}

		if (EnemyInitialLocation.Equals(getEnemy()->GetActorLocation(), 10.f))
		{
			if (getEnemy()->IsImmortal())
			{
				OgichiRef->SkillEnable();
				OgichiRef->Btn_Bankai();
				Intro = false;
				Wait(3.f);
			}
		}
		else
		{
			Intro = false;
			Cast<AHeroBase>(getEnemy())->getHeroStatsComp()->SetSkill("Bankai", false);
		}
		return;
	}

	switch (OgichiRef->GetState())
	{
	case EBaseStates::Stand:
	{

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
			if (getEnemy()->GetDangerType() == EDangerType::MeleeAttack
				&& ((isEnemyOnRight() && !getEnemy()->IsLookingRight()) || (!isEnemyOnRight() && getEnemy()->IsLookingRight()))
				)
			{
				OgichiRef->SetMoveVector(getForwardVector());
				OgichiRef->Block();
				Wait(0.2f);
				return;
			}

			// Teleprot to behind of enemy
			if (UKismetMathLibrary::RandomBoolWithWeight(TP_CHANCE) && canTeleport)
			{
				OgichiRef->Teleport(getEnemyLocation() + ((isEnemyOnRight()) ? FVector(50.f, 0.f, 0.f) : FVector(-50.f, 0.f, 0.f)));
				tpReload(0.5f);
				return;
			}

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
			if (UKismetMathLibrary::RandomBoolWithWeight(0.01f) 
				&& OgichiRef->getHeroStatsComp()->checkStamina(-(GETSUGA_COST))
				&& OgichiRef->getHeroStatsComp()->checkPower(-(GETSUGA_COST))
				&& getDistanceY() < MIN_DIST_Y)
			{
				OgichiRef->SetRotation(isEnemyOnRight(), false);
				OgichiRef->SkillEnable();
				OgichiRef->AttackForward();
				return;
			}

			return;
		}

		return;
	}

	case EBaseStates::Blocking:
	{
		if (getEnemy()->GetDangerType() == EDangerType::None)
			OgichiRef->BlockStop();
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
		break;
	}

	case EBaseStates::Fall:
	{
		if (OgichiRef->GetVelocity().Z > 20.f)
		{
			OgichiRef->StandUp();
		}
	}

	case EOgichiState::Ogi_Attack_1:
	{
		if (!OgichiRef->isComboTime()) { return; }

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
		break;
	}

	case EOgichiState::Ogi_Attack_2:
	{
		if (!OgichiRef->isComboTime())
			return;

		if (getDistanceY() > MAX_DIST_Y)
			return;

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
	}

	case EOgichiState::Ogi_SwordTwistLoop:
	{
		if (getEnemy()->IsFalling())
		{
			OgichiRef->Attack();
			return;
		}

		if (getDistanceY() > MAX_DIST_Y)
			return;

		// Ogi_Getsuga
		if (getDistanceX() > MAX_DIST_X && getDistanceX() < MAX_DIST_X * 4
			&& UKismetMathLibrary::RandomBoolWithWeight(0.2f)
			&& OgichiRef->getHeroStatsComp()->checkStamina(-(GETSUGA_TENSHOU_COST))
			&& OgichiRef->getHeroStatsComp()->checkPower(-(GETSUGA_TENSHOU_COST))
			)
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
		break;
	}



	} // End Switch



}
