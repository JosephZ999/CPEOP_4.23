// Fill out your copyright notice in the Description page of Project Settings.

#include "ShriekerSummon.h"

AShriekerSummon::AShriekerSummon()
{
	AddAnimation(FName("Stand"), nullptr);
	AddAnimation(FName("Jumping"), nullptr);
	AddAnimation(FName("Attack_1"), nullptr);
	AddAnimation(FName("Attack_2"), nullptr);
	AddAnimation(FName("Hit"), nullptr);

	InitHelper("ShriekerSummonSlash");
	InitHelper("ShriekerSummonSlash2");

	CanFall = false;
}

void AShriekerSummon::EndState()
{
	Super::EndState();
	AnimUpdate();
}

void AShriekerSummon::AnimUpdate()
{
	switch (GetState())
	{
	case EBaseStates::Stand:
	{
		SetAnim(FName("Stand"), false);
		break;
	}
	case EBaseStates::Jumping:
	{
		SetAnim(FName("Jumping"), false);
		break;
	}
	}
}

void AShriekerSummon::LandAttack()
{
	if (! IsOnGround() || ! CheckState(EBaseStates::Stand))
		return;

	FState nState;
	nState.State	 = EMonsterStates::M_Attack_1;
	nState.Animation = "Attack_1";
	NewState(nState);

	SpawnHelperDeferred("ShriekerSummonSlash", AnimElemTime(4));
}

void AShriekerSummon::JumpAttack()
{
	if (IsOnGround() || ! CheckState(EBaseStates::Jumping))
		return;

	if (GetUnitVelocity().Z < 50.f)
		return;

	FState nState;
	nState.State	 = EMonsterStates::M_Attack_2;
	nState.Animation = "Attack_2";
	NewState(nState);

	SpawnHelperDeferred("ShriekerSummonSlash2", AnimElemTime(3));
}

void AShriekerSummon::JumpTo(FVector forwardVector)
{
	if (! IsOnGround() || ! CheckState(EBaseStates::Stand))
		return;

	FState nState;
	nState.State	 = EBaseStates::Dash;
	nState.Animation = "Stand";
	nState.EndState	 = false;
	NewState(nState);

	float Delay = 0.2f + FMath::RandRange(0.f, 0.05f);

	AddImpulse(forwardVector * (200.f - FMath::RandRange(0.f, 30.f)), Delay);
	EndStateDeferred(Delay + 0.01f);
}
