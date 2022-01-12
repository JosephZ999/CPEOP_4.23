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

	switch (HType)
	{
	case EHollowType::Default:  { AITypeDef();      break; }
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
		}
	}
	}
}

void AHollowAI_1::AITypeRanged()
{
	switch (OwnerRef->getState())
	{
	case EBaseStates::Stand:
	{
		if (getDistanceX() < MinDistX || getDistanceX() > MaxDistX || getDistanceY() > 20.f)
		{
			OwnerRef->SetMoveVector(getForwardVector(FMath::Lerp(MinDistX, MaxDistX, 0.5f)));
		}
		else
		{
			OwnerRef->SetMoveVector(FVector(0.f));
			OwnerRef->SetRotation(isEnemyOnRight(), false);
			OwnerRef->Attack();
		}
	}
	}

}