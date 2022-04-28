#include "Ichigo.h"
#include "ShadowComponent.h"

//----------------------------------------------/ Input

void AIchigo::b_InputA()
{
	switch (GetState())
	{
	case EBaseStates::Stand: b_Attack_1(); break;

	case EBaseStates::Jumping: b_Attack_Air(); break;

	case EIchigoState::Ichi_Attack_1:
		if (isComboTime())
		{
			b_Attack_2();
		}
		break;

	case EIchigoState::Ichi_Attack_2:
		if (isComboTime())
		{
			b_Attack_3();
		}
		break;

	case EIchigoState::Ichi_Attack_3:
		if (isComboTime())
		{
			b_Attack_4();
		}
		break;

	} // Switch End
}

void AIchigo::b_InputB()
{
	switch (GetState())
	{
	case EBaseStates::Stand:
		if (IsSkillActive())
		{
			b_RExplosion();
		}
		else
		{
			b_Attack_B();
		}
		break;

	case EIchigoState::Ichi_Attack_2:
		if (isComboTime())
		{
			b_Attack_B();
		}
		break;

	case EIchigoState::Ichi_Attack_3:
		if (isComboTime())
		{
			b_Attack_B();
		}
		break;

	case EBaseStates::PowChargeLoop: b_RExplosion(); break;

	} // Switch End
}

void AIchigo::b_InputFW()
{
	switch (GetState())
	{
	case EBaseStates::Stand:
		if (IsSkillActive())
		{
			b_Getsuga();
		}
		else
		{
			b_Attack_FW();
		}
		break;

	case EIchigoState::Ichi_Attack_2:
		if (isComboTime())
		{
			b_Attack_FW();
		}
		break;

	case EIchigoState::Ichi_Attack_3:
		if (isComboTime())
		{
			b_Attack_FW();
		}
		break;

	case EBaseStates::PowChargeLoop: b_Getsuga(); break;

	} // End Switch
}

void AIchigo::b_InputD()
{
	switch (GetState())
	{
	case EBaseStates::Stand:
		if (IsSkillActive())
		{
			b_Shikai();
		}
		break;

	case EBaseStates::PowChargeLoop: b_Shikai(); break;

	} // End Switch
}

// End Input /----------------------------------/

//----------------------------------------------/ Action

void AIchigo::b_Attack_1()
{
	FState nState;
	nState.State	 = EIchigoState::Ichi_Attack_1;
	nState.Animation = "Attack_1";
	NewState(nState);

	AddImpulse(BASE_VELOCITY, getFrameTime(1));
	SpawnHelperDeferred("b_Attack_1", getFrameTime(3));
	Combo(getFrameTime(7));

	SetBlockingAttack(EBlockType::Forward, getFrameTime(3), BLOCK_DURATION);
	DangerN(getFrameTime(6), EDangerType::MeleeAttack);
}

void AIchigo::b_Attack_2()
{
	FState nState;
	nState.State	 = EIchigoState::Ichi_Attack_2;
	nState.Animation = "Attack_2";
	NewState(nState);

	AddImpulse(BASE_VELOCITY, getFrameTime(1));
	SpawnHelperDeferred("b_Attack_2", getFrameTime(3));
	Combo(getFrameTime(7));

	SetBlockingAttack(EBlockType::Forward, getFrameTime(3), BLOCK_DURATION);
	DangerN(getFrameTime(6), EDangerType::MeleeAttack);
}

void AIchigo::b_Attack_3()
{
	FState nState;
	nState.State	 = EIchigoState::Ichi_Attack_3;
	nState.Animation = "Attack_3";
	NewState(nState);

	AddImpulse(BASE_VELOCITY, getFrameTime(3));
	SpawnHelperDeferred("b_Attack_3", getFrameTime(4));
	Combo(getFrameTime(9));

	SetBlockingAttack(EBlockType::Both, getFrameTime(3), getFrameTime(4));
	DangerN(getFrameTime(6), EDangerType::MeleeAttack);
}

void AIchigo::b_Attack_4()
{
	FState nState;
	nState.State	 = EIchigoState::Ichi_Attack_4;
	nState.Animation = "Attack_4";
	NewState(nState);

	AddImpulse(BASE_VELOCITY, getFrameTime(2));
	SpawnHelperDeferred("b_Attack_4", getFrameTime(3));
	Combo(getFrameTime(10));

	SetBlockingAttack(EBlockType::Forward, getFrameTime(3), BLOCK_DURATION);
	DangerN(getFrameTime(4), EDangerType::MeleeAttack);
}

void AIchigo::b_Attack_FW()
{
	bool EnoughPower = GetHeroStats()->CheckPower(0.f, 2.f / GetHeroStats()->GetTeleportCost());
	if (! EnoughPower)
	{
		return;
	}

	FState nState;
	nState.State	 = EIchigoState::Ichi_Attack_FW;
	nState.Animation = "Attack_FW";
	NewState(nState);

	SpawnHelperDeferred("b_Attack_FW", getFrameTime(5));
	SpawnHelperDeferred("Teleport", getFrameTime(4), GetActorRotation());
	SetCameraViewA(GetCameraLocation(), getFrameTime(12));

	GET_STATS->AddStamina(-2.f / GetHeroStats()->GetTeleportCost(), getFrameTime(4), false, EIchigoState::Ichi_Attack_FW);

	SetBlockingAttack(EBlockType::Forward, getFrameTime(4), BLOCK_DURATION);
	DangerN(getFrameTime(5), EDangerType::MeleeAttack);
	Combo(getFrameTime(11));

	DashStartLoc = GetActorLocation();
	PlayTimeline(this, b_AttackDashCurve, "b_AttackDash", false);
}

void AIchigo::b_AttackDash(float value)
{
	FVector TargetLocation = DashStartLoc;
	if (IsLookingRight())
	{
		TargetLocation.X = FMath::Lerp(TargetLocation.X, TargetLocation.X + 200.f, value);
	}
	else
	{
		TargetLocation.X = FMath::Lerp(TargetLocation.X, TargetLocation.X - 200.f, value);
	}
	SetActorLocation(TargetLocation, true);
	if (! CheckState(EIchigoState::Ichi_Attack_FW))
	{
		StopTimeline();
	}
}

void AIchigo::b_Attack_FW_Slash()
{
	bool EnoughPower = GetHeroStats()->CheckPower(0.f, 1.f / GetHeroStats()->GetTeleportCost());
	if (! EnoughPower)
	{
		if (CheckState(EIchigoState::Ichi_Attack_FW_Slash))
		{
			b_Attack_FW_End();
		}
		return;
	}

	if (CheckState(EIchigoState::Ichi_Attack_FW))
	{
		b_SlashLocation = GetActorLocation();
		FAfterImageStruct nImg("Stand", GetActorLocation(), IsLookingRight(), 0);
		AfterImage->Create(nImg, 0.f);
	}

	FState nState;
	nState.Animation = "Hide";
	nState.EndState	 = false;
	nState.Rotate	 = false;
	nState.State	 = EIchigoState::Ichi_Attack_FW_Slash;
	NewState(nState);

	GET_STATS->AddStamina(-1.f / GetHeroStats()->GetTeleportCost());
	SetCameraViewA(GetCameraViewPosition(), 0.2f);
	getShadow()->HideShadow();

	SpawnHelperDeferred("b_Attack_FW_Slash", 0.04f);
	SpawnHelperDeferred("b_Attack_FW_Slash", 0.08f);
	SpawnHelperDeferred("b_Attack_FW_Slash", 0.12f);

	SetActorLocation(b_SlashLocation + FVector(120.f * ((IsLookingRight()) ? -1 : 1.f), 0.f, 0.f), true);

	for (int i = 1; i <= 2; i++)
	{
		bool	OnRight = (i % 2 == 0);
		FVector nLoc(GetActorLocation());
		nLoc.X += (OnRight) ? FMath::RandRange(100.f, 150.f) : FMath::RandRange(-100.f, -150.f);
		nLoc.Y += FMath::RandRange(-75.f, 75.f);

		FAfterImageStruct nImg("Run", nLoc, true, 0);
		nImg.Rotation = (! OnRight);
		AfterImage->Create(nImg, i * 0.04f);
	}

	SetImmortality(0.12f);
	Combo(0.12f);
}

void AIchigo::b_Attack_FW_End()
{
	getShadow()->ShowShadow();

	SetActorLocation(b_SlashLocation + FVector(200.f * ((IsLookingRight()) ? -1 : 1.f), 0.f, 0.f), true);
	SpawnHelperDeferred("Teleport", 0.f, GetActorRotation());

	EndStateDeferred(0.1f);
}

void AIchigo::b_Attack_B()
{
	FState nState;
	nState.State	 = EIchigoState::Ichi_Attack_B;
	nState.Animation = "Attack_B";
	NewState(nState);

	AddImpulse(SP_VELOCITY, getFrameTime(4));
	SpawnHelperDeferred("b_Attack_B", getFrameTime(6));
	Combo(getFrameTime(11));

	SetBlockingAttack(EBlockType::Forward, getFrameTime(5), BLOCK_DURATION);
	DangerN(getFrameTime(6), EDangerType::MeleeAttack);
}

void AIchigo::b_Attack_Air()
{
	FState nState;
	nState.State	 = EIchigoState::Ichi_Attack_Air;
	nState.Animation = "AttackAir";
	NewState(nState);

	AddImpulse(FVector(GetUnitVelocity().X, GetUnitVelocity().Y, 200.f), getFrameTime(3));
	SpawnHelperDeferred("b_AttackAir", getFrameTime(4));
	Combo(getFrameTime(12));

	SetBlockingAttack(EBlockType::Forward, getFrameTime(4), BLOCK_DURATION);
	DangerN(getFrameTime(6), EDangerType::MeleeAttack);
}

void AIchigo::b_Getsuga()
{

	if (! GetHeroStats()->CheckSkill("Getsuga"))
		return;

	bool EnoughPower = GetHeroStats()->CheckPower(-(GETSUGA_COST), -(GETSUGA_COST), true);
	if (! EnoughPower)
	{
		return;
	}

	FState nState;
	nState.State	 = EIchigoState::Ichi_GetsugaStart;
	nState.Animation = "GetsugaReady";
	NewState(nState);

	AddImpulse(MoveVector * 300.f, 0.f);
	SpawnHelperDeferred("b_GetsugaEff", getFrameTime(2));
	GET_STATS->AddStamina(GETSUGA_COST, AnimElemTime(9), true, EIchigoState::Ichi_GetsugaFW);

	SetBlockingAttack(EBlockType::Both, 0.f, getFrameTime(10));
	Combo(getFrameTime(6));
	SkillDisable();
}

void AIchigo::b_GetsugaFW()
{
	FState nState;
	nState.State	 = EIchigoState::Ichi_GetsugaFW;
	nState.Animation = "Getsuga";
	nState.Rotate	 = false;
	NewState(nState);

	SpawnHelperDeferred("b_Getsuga", getFrameTime(2));
	Combo(getFrameTime(8));
}

void AIchigo::b_GetsugaB()
{
	FState nState;
	nState.State	 = EIchigoState::Ichi_GetsugaFW;
	nState.Animation = "GetsugaB";
	nState.Rotate	 = false;
	NewState(nState);

	SpawnHelperDeferred("b_Getsuga", getFrameTime(2), FRotator(45.f, 0.f, 0.f));
	Combo(getFrameTime(8));
}

void AIchigo::b_RExplosion()
{
	if (! GetHeroStats()->CheckSkill("RExplosion"))
		return;

	bool EnoughPower = GetHeroStats()->CheckPower(-(EXPLOSION_COST), -(EXPLOSION_COST), true);
	if (! EnoughPower)
	{
		return;
	}

	FState nState;
	nState.State	 = EIchigoState::Ichi_RExplosion;
	nState.Animation = "RExplosion";
	NewState(nState);

	AddImpulse(MoveVector * 300.f, 0.f);
	SpawnHelperDeferred("b_RExplosion", getFrameTime(7));
	GET_STATS->AddStamina(EXPLOSION_COST, AnimElemTime(7), true, EIchigoState::Ichi_RExplosion);

	SetBlockingAttack(EBlockType::Both, 0.f, AnimElemTime(14));
	Combo(getFrameTime(16));
	SkillDisable();
}

void AIchigo::b_Shikai()
{
	if (! GetHeroStats()->CheckSkill("Bankai"))
		return;

	FState nState;
	nState.State	 = EIchigoState::Ichi_Shikai;
	nState.Animation = "Shikai";
	NewState(nState);

	Shikai();
	Combo(getFrameTime(8));
	SetImmortality(AnimElemTime(4));
	GET_STATS->SetExpMultiplier(1.f);
	SkillDisable();
}

// End Action /---------------------------------/

//----------------------------------------------/ Combo

void AIchigo::BankaiComboI()
{
	EComboKey key = getNextKey();

	switch (GetState())
	{
	case EIchigoState::Ichi_Attack_1:
	{
		switch (key)
		{
		case EComboKey::CK_Attack:
		{
			b_Attack_2();
			break;
		}
		case EComboKey::CK_Dash:
		{
			DoDash();
			break;
		}
		}
		break;
	}
	case EIchigoState::Ichi_Attack_2:
	{
		switch (key)
		{
		case EComboKey::CK_Attack:
		{
			b_Attack_3();
			break;
		}
		case EComboKey::CK_ABackward:
		{
			b_Attack_B();
			break;
		}
		case EComboKey::CK_AForward:
		{
			b_Attack_FW();
			break;
		}
		case EComboKey::CK_Dash:
		{
			DoDash();
			break;
		}
		}
		break;
	}
	case EIchigoState::Ichi_Attack_3:
	{
		switch (key)
		{
		case EComboKey::CK_Attack:
		{
			b_Attack_4();
			break;
		}
		case EComboKey::CK_ABackward:
		{
			b_Attack_B();
			break;
		}
		case EComboKey::CK_AForward:
		{
			b_Attack_FW();
			break;
		}
		case EComboKey::CK_Dash:
		{
			DoDash();
			break;
		}
		}
		break;
	}
	case EIchigoState::Ichi_Attack_4:
	{
		switch (key)
		{
		case EComboKey::CK_Dash:
		{
			DoDash();
			break;
		}
		}
		break;
	}

	case EIchigoState::Ichi_Attack_B:
	{
		switch (key)
		{
		case EComboKey::CK_Dash:
		{
			DoDash();
			break;
		}
		}
		break;
	}
	case EIchigoState::Ichi_Attack_FW:
	{
		switch (key)
		{
		case EComboKey::CK_Attack:
		{
			b_Attack_FW_Slash();
			break;
		}
		case EComboKey::CK_Dash:
		{
			DoDash();
			break;
		}
		}
		break;
	}
	case EIchigoState::Ichi_Attack_FW_Slash:
	{
		switch (key)
		{
		case EComboKey::CK_Attack:
		{
			b_Attack_FW_Slash();
			break;
		}
		default:
		{
			b_Attack_FW_End();
			break;
		}
		}
		break;
	}
	case EIchigoState::Ichi_GetsugaStart:
	{
		switch (key)
		{
		case EComboKey::CK_ABackward:
		{
			b_GetsugaB();
			break;
		}
		default:
		{
			b_GetsugaFW();
			break;
		}
		}
		break;
	}
	case EIchigoState::Ichi_GetsugaFW:
	{
		switch (key)
		{
		case EComboKey::CK_Dash:
		{
			DoDash();
			break;
		}
		}
		break;
	}
	} // Switch End
}

// End Combo /----------------------------------/
