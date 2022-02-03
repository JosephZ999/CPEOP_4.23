// Fill out your copyright notice in the Description page of Project Settings.


#include "ShriekerSummonAI.h"
#include "Chars/Monsters/ShriekerSummon.h"

AShriekerSummonAI::AShriekerSummonAI()
{
	SearchStepRadius = 100.f;
	SearchSteps = 10;
}

void AShriekerSummonAI::AIBody()
{
	Super::AIBody();

	if (!OwnerRef)
	{
		OwnerRef = Cast<AShriekerSummon>(GetPawn());
		return;
	}

	if (OwnerRef->IsDead())
		return;

	if (!getEnemy())
	{
		if (!SearchEnemy(OwnerRef->GetTeam()))
		{
			OwnerRef->SetMoveVector(FVector::ZeroVector);
			Wait(1.f);
			return;
		}
	}

	switch (OwnerRef->GetState())
	{
	case EBaseStates::Stand:
	{
		if (OwnerRef->IsOnGround())
		{
			if (getDistance() > 50.f)
			{
				FVector nImpulse = getForwardVector();
				nImpulse.Z = 1.f;
				OwnerRef->JumpTo(nImpulse);
				OwnerRef->SetRotation(isEnemyOnRight(), false);
				return;
			}

			// Move away if it's not first attack
			if (AttackSuccess)
			{
				FVector nImpulse = FVector(((FMath::RandBool()) ? 1.f : -1.f), 0.f,	0.8f);
				OwnerRef->JumpTo(nImpulse);
				OwnerRef->SetRotation(nImpulse.X > 0.f, false);
				AttackSuccess = false;
				Wait(1.f);
				return;
			}

			// Land Attack
			OwnerRef->SetRotation(isEnemyOnRight(), false);
			OwnerRef->LandAttack();
			return;
		}
		else if (getDistance() <= 75.f && getDistanceY() <= 30.f)
		{
			// Jump Attack
			OwnerRef->SetRotation(isEnemyOnRight(), false);
			OwnerRef->JumpAttack();
			return;
		}
		break;
	}
	case EBaseStates::Jumping:
	{
		if (!OwnerRef->IsOnGround() && getDistanceX() <= 75.f && getDistanceY() <= 30.f)
		{
			// Jump Attack
			OwnerRef->SetRotation(isEnemyOnRight(), false);
			OwnerRef->JumpAttack();
			return;
		}
		break;
	}
	case EMonsterStates::M_Attack_1:
	{
		AttackSuccess = true;
		SET_TIMER(AttackTimer, this, &AShriekerSummonAI::AttackSuccessReset, 2.f);
		Wait(0.5f);
	}
	} // Switch end
}

void AShriekerSummonAI::AttackSuccessReset()
{
	AttackSuccess = false;
}

