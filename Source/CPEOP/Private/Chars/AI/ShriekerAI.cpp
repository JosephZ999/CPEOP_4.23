// Fill out your copyright notice in the Description page of Project Settings.


#include "ShriekerAI.h"
#include "Chars/Monsters/Shrieker.h"

AShriekerAI::AShriekerAI()
{
	OwnerRef = nullptr;
	Action = EActionType::GetDistance;
	DesiredLocation = FVector(0.f);
	DistanceMax = 200.f;
	DistanceMin = 300.f;
	AttackDistance = 100.f;

	SearchStepRadius = 100.f;
	SearchSteps = 15;

	MoveAwayProgress = 0.f;
}

void AShriekerAI::AIBody()
{
	if (!OwnerRef)
	{
		OwnerRef = Cast<AShrieker>(GetPawn());
		return;
	}

	if (!SearchEnemy(OwnerRef->GetTeam()))
	{
		Wait(1.f);
		return;
	}

	if (OwnerRef->IsDead())
	{
		StopAI();
		return;
	}

	switch (Action)
	{
	case EActionType::Attack:
	{
		ActionMode(); break;
	}
	case EActionType::GetDistance:
	{
		GetDistanceMode(); break;
	}
	case EActionType::Summon:
	{
		Summon(); break;
	}

	} // Switch End
}

void AShriekerAI::StopMoving()
{
	OwnerRef->SetMoveVector(FVector(0.f));
}

// Actions

void AShriekerAI::ActionMode()
{
	DesiredLocation = getEnemyLocation(AttackDistance * 0.5f);

	if (!FVector::PointsAreNear(OwnerRef->GetActorLocation(), DesiredLocation, AttackDistance))
	{
		OwnerRef->SetMoveVector(
			getForwardVector(OwnerRef->GetActorLocation(), DesiredLocation)
		);
	}
	else
	{
		OwnerRef->SetRotation(isEnemyOnRight());
		OwnerRef->SetMoveVector(getForwardVector());
		OwnerRef->Attack();

		Action = EActionType::GetDistance;
		Wait(1.f);
		return;
	}
}

void AShriekerAI::GetDistanceMode()
{
	if (CanOwnerMoveAway())
	{
		DesiredLocation = getEnemyLocation(DistanceMax);
	}

	DesiredLocation.Z = 150.f;

	if (!FVector::PointsAreNear(OwnerRef->GetActorLocation(), DesiredLocation, AttackDistance))
	{
		OwnerRef->SetMoveVector(
			getForwardVector(OwnerRef->GetActorLocation(), DesiredLocation)
		);

		if (FMath::IsNearlyZero(OwnerRef->GetUnitVelocity().X, 1.f))
		{
			MoveAwayProgress = FMath::Min(MoveAwayProgress + 1.f, 20.f);

			if (!CanOwnerMoveAway() && !IsMovingAway)
			{
				MoveAwayProgress = 50.f;
				DesiredLocation = getEnemyLocation(-DistanceMax);
				IsMovingAway = true;
				return;
			}
		}
		else
		{
			MoveAwayProgress = FMath::Max(MoveAwayProgress - 0.1f, 0.f);

			if (MoveAwayProgress <= 1.f)
			{
				IsMovingAway = false;
			}
		}
	}
	else
	{
		OwnerRef->SetRotation(isEnemyOnRight());
		StopMoving();
		MoveAwayProgress = 0.f;
		IsMovingAway = false;

		if (OwnerRef->SummonsNum == 0)
		{
			Action = EActionType::Attack;
		}
		else
		{
			Action = EActionType::Summon;
			OwnerRef->Summon();
		}

		Wait(0.5f);
		return;
	}
}


void AShriekerAI::Summon()
{
	if (CanOwnerMoveAway())
	{
		DesiredLocation = getEnemyLocation(DistanceMax);
	}

	DesiredLocation.Z = 150.f;

	if (!FVector::PointsAreNear(OwnerRef->GetActorLocation(), DesiredLocation, AttackDistance))
	{
		OwnerRef->SetMoveVector(
			getForwardVector(OwnerRef->GetActorLocation(), DesiredLocation)
		);

		if (FMath::IsNearlyZero(OwnerRef->GetUnitVelocity().X, 1.f))
		{
			MoveAwayProgress = FMath::Min(MoveAwayProgress + 1.f, 20.f);

			if (!CanOwnerMoveAway() && !IsMovingAway)
			{
				MoveAwayProgress = 50.f;
				DesiredLocation = getEnemyLocation(-DistanceMax);
				IsMovingAway = true;
				return;
			}
		}
		else
		{
			MoveAwayProgress = FMath::Max(MoveAwayProgress - 0.1f, 0.f);

			if (MoveAwayProgress <= 1.f)
			{
				IsMovingAway = false;
			}
		}
	}
	else
	{
		OwnerRef->SetRotation(isEnemyOnRight());
		StopMoving();
		MoveAwayProgress = 0.f;
		IsMovingAway = false;
	}

	if (OwnerRef->SummonsNum == 0)
	{
		Action = EActionType::Attack;
		Wait(3.f);
	}

	if (
		(OwnerRef->getStats()->GetHealth() / OwnerRef->getStats()->GetMaxHealth()) < 0.3f
		)
	{
		Action = EActionType::Attack;
		Wait(0.5f);
	}
}