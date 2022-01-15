// Fill out your copyright notice in the Description page of Project Settings.


#include "Chars/AI/HollowAI_1.h"
#include "Chars/MonsterBase.h"

void AHollowAI_1::InitParams(EHollowType type, float minDistX, float maxDistX)
{
	HType = type;
	MinDistX = minDistX;
	MaxDistX = maxDistX;
}

void AHollowAI_1::AIBody()
{
	if (!OwnerRef)
	{
		OwnerRef = Cast<AMonsterBase>(GetPawn());
		return;
	}

	// Search Enemy
	if (!getEnemy())
	{
		if (!SearchEnemy(OwnerRef->getTeam()))
		{
			Wait(1.f);
			return;
		}
	}

	if (OwnerRef->IsDead())
	{
		OwnerRef->SetMoveVector(FVector(0.f));
		return;
	}

	switch (HType)
	{
	case EHollowType::Melee:    { AITypeDef();      break; }
	case EHollowType::Ranged:   { AITypeRanged();   break; }

	} // Switch End

}

void AHollowAI_1::AITypeDef()
{
	switch (OwnerRef->getState())
	{
	case EBaseStates::Stand:
	{
		if (getDistanceX() > MinDistX || getDistanceY() > 20.f)
		{
			OwnerRef->SetMoveVector(getForwardVector(MinDistX));
		}
		else
		{
			OwnerRef->SetMoveVector(FVector(0.f));
			OwnerRef->SetRotation(isEnemyOnRight(), false);
			OwnerRef->Attack();
			if (OwnerRef->getState() == EMonsterStates::Attack_1)
			{
				FVector vec = (getForwardVector() * getDistance() * 5.f);
				OwnerRef->AddImpulse(FVector(vec.X, vec.Y, 20.f), 0.1f);
			}
		}
		break;
	}
	case EMonsterStates::Attack_1:
	{
		if (OwnerRef->ComboTime())
		{
			StopMoving();
			Wait(0.5f + FMath::FRandRange(0.f, 0.5f));
		}
		break;
	}

	case EBaseStates::Hit:
	{
		StopMoving();
		Wait(0.5f + FMath::FRandRange(0.f, 0.5f));
		break;
	}

	} // Switch End
}

void AHollowAI_1::AITypeRanged()
{
	switch (OwnerRef->getState())
	{
	case EBaseStates::Stand:
	{
		if ((getDistanceX() < MinDistX || getDistanceX() > MaxDistX) || getDistanceY() > 40.f)
		{
			OwnerRef->SetMoveVector(getForwardVector(FMath::Lerp(MinDistX, MaxDistX, 0.5f)));
		}
		else
		{
			StopMoving();
			OwnerRef->SetRotation(isEnemyOnRight(), false);
			OwnerRef->Attack();
			if (OwnerRef->getState() == EMonsterStates::Attack_1)
			{
				FVector vel{ 0.f, 0.f, 20.f };
				vel.Y = getDistanceY() * 5 * ((getEnemyLocation().Y > getPawnLocation().Y) ? 1.f : -1.f);
				OwnerRef->AddImpulse(vel);
			}
		}
		break;
	}
	case EMonsterStates::Attack_1:
	{
		if (OwnerRef->ComboTime())
		{
			StopMoving();
			Wait(0.5f + FMath::FRandRange(0.f, 0.5f));
		}
		break;
	}

	case EBaseStates::Hit:
	{
		StopMoving();
		Wait(0.5f + FMath::FRandRange(0.f, 0.5f));
		break;
	}

	} // Switch End

}

void AHollowAI_1::StopMoving()
{
	OwnerRef->SetMoveVector(FVector(0.f));
}