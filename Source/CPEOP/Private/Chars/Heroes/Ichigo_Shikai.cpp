#include "Chars/Heroes/Ichigo.h"
#include "Chars/Components/ShadowComponent.h"

//----------------------------------------------/ Input

void AIchigo::sh_InputA()
{
	switch (GetState())
	{
	case EBaseStates::Stand:			{ sh_Attack_1(); break; }
	case EBaseStates::Jumping:			{ sh_Attack_Air(); break; }
	case EIchigoState::Ichi_Attack_1:
	{
		if (isComboTime()) 
		{ 
			sh_Attack_2(); 
		}
		break;
	}
	case EIchigoState::Ichi_Attack_FW:	{ SkillEnable();	break; }
	case EIchigoState::Ichi_Attack_B:	{ SkillEnable(); break; }
	case EIchigoState::Ichi_SwordTwistLoop:
	{
		SET_TIMER(sh_STwistEndTimer, this, &AIchigo::sh_SwordTwistEnd, cTime(1.f));
		GET_STATS->AddStamina(0.07);
		break;
	}
	} // End Switch
}

void AIchigo::sh_InputB()
{
	switch (GetState())
	{
	case EBaseStates::Stand:
	{
		if (IsSkillActive())
		{
			sh_SwordTwist(); 
			SkillDisable();
		}
		else
		{
			sh_Attack_B();
		}
		break;
	}
	case EIchigoState::Ichi_Attack_2: 
	{ 
		if (isComboTime()) 
		{ 
			sh_Attack_B(); 
		}
		break; 
	}
	case EBaseStates::PowChargeLoop:		{ sh_SwordTwist(); break; }
	case EIchigoState::Ichi_SwordTwistLoop:
	{
		if (IsSkillActive())
		{
			sh_RExplosion(); 
			SkillDisable();
		}
		else
		{
			sh_Attack_B();
		}
		break;
	}
	} // End Switch
}

void AIchigo::sh_InputFW()
{
	switch (GetState())
	{
	case EBaseStates::Stand:
	{
		if (IsSkillActive())
		{
			sh_GetsugaStart();
		}
		else
		{
			sh_Attack_FW();
		}
		break;
	}
	case EIchigoState::Ichi_Attack_2: 
	{ 
		if (isComboTime()) 
		{ 
			sh_Attack_FW(); 
		} 
		break; 
	}
	case EBaseStates::PowChargeLoop:		{ sh_GetsugaStart(); break; }
	case EIchigoState::Ichi_SwordTwistLoop:
	{
		if (IsSkillActive())
		{

		}
		else
		{
			sh_SwordThrow();
		}
		break;
	}
	} // End Switch
}

void AIchigo::sh_InputD()
{
	switch (GetState())
	{
	case EBaseStates::Stand: 
	{ 
		if (IsSkillActive()) 
		{ 
			sh_Bankai(); 
		} 
		break; 
	}
	case EBaseStates::PowChargeLoop: { sh_Bankai(); break; }
	}
}

// End Input /----------------------------------/

//----------------------------------------------/ Action

void AIchigo::sh_Attack_1()
{
	FState nState;
	nState.State = EIchigoState::Ichi_Attack_1;
	nState.Animation = "Attack_1";
	NewState(nState);

	AddImpulse(BASE_VELOCITY, getFrameTime(2));
	SpawnHelper("sh_Attack_1", getFrameTime(4));
	Combo(getFrameTime(9));

	SetBlockingAttack(EBlockType::Forward, getFrameTime(4), BLOCK_DURATION);
	DangerN(getFrameTime(6), EDangerType::MeleeAttack);

}

void AIchigo::sh_Attack_2()
{
	FState nState;
	nState.State = EIchigoState::Ichi_Attack_2;
	nState.Animation = "Attack_2";
	NewState(nState);

	AddImpulse(BASE_VELOCITY, getFrameTime(1));
	SpawnHelper("sh_Attack_2", getFrameTime(4));
	Combo(getFrameTime(9));

	SetBlockingAttack(EBlockType::Forward, getFrameTime(4), BLOCK_DURATION);
	DangerN(getFrameTime(6), EDangerType::MeleeAttack);
}

void AIchigo::sh_Attack_Air()
{
	FState nState;
	nState.State = EIchigoState::Ichi_Attack_Air;
	nState.Animation = "AttackAir";
	NewState(nState);

	AddImpulse(FVector(GetUnitVelocity().X, GetUnitVelocity().Y, 200.f), getFrameTime(4));
	SpawnHelper("sh_AttackAir", getFrameTime(4));
	Combo(getFrameTime(12));

	SetBlockingAttack(EBlockType::Forward, getFrameTime(4), BLOCK_DURATION);
	DangerN(getFrameTime(6), EDangerType::MeleeAttack);
}

void AIchigo::sh_Attack_FW()
{
	FState nState;
	nState.State = EIchigoState::Ichi_Attack_FW;
	nState.Animation = "AttackFW";
	NewState(nState);

	AddImpulse(SP_VELOCITY, getFrameTime(1));
	SpawnHelper("sh_AttackForward", getFrameTime(5));
	Combo(getFrameTime(12));

	SetBlockingAttack(EBlockType::Forward, getFrameTime(5), BLOCK_DURATION);
	DangerN(getFrameTime(7), EDangerType::MeleeAttack);

	FTimerHandle nTimer;
	SET_TIMER(nTimer, this, &AIchigo::sh_GetsugaFW, getFrameTime(5));
}

void AIchigo::sh_GetsugaFW()
{
	if (!CheckState(EIchigoState::Ichi_Attack_FW))
		return;

	if (!getHeroStatsComp()->CheckSkill("Getsuga"))
		return;

	if (IsSkillActive())
	{
		bool Return{ false };
		if (!getHeroStatsComp()->checkStamina(-(GETSUGA_COST)))
		{
			NotEnoughStamina();
			Return = true;
		}
		if (!getHeroStatsComp()->checkPower(-(GETSUGA_COST)))
		{
			NotEnoughPower();
			Return = true;
		}
		if (Return) { return; }

		GET_STATS->AddStamina(GETSUGA_COST, 0.f, true);
		SpawnHelper("sh_GetsugaFWHelper");
		SkillDisable();
	}
}

void AIchigo::sh_Attack_B()
{
	FState nState;
	nState.State = EIchigoState::Ichi_Attack_B;
	nState.Animation = "AttackB";
	NewState(nState);

	AddImpulse(SP_VELOCITY, getFrameTime(4));
	SpawnHelper("sh_AttackBack", getFrameTime(6));
	Combo(getFrameTime(14));

	SetBlockingAttack(EBlockType::Forward, getFrameTime(6), BLOCK_DURATION);
	DangerN(getFrameTime(8), EDangerType::MeleeAttack);

	FTimerHandle nTimer;
	SET_TIMER(nTimer, this, &AIchigo::sh_GetsugaB, getFrameTime(6));
}

void AIchigo::sh_GetsugaB()
{
	if (!CheckState(EIchigoState::Ichi_Attack_B))
		return;

	if (!getHeroStatsComp()->CheckSkill("Getsuga"))
		return;

	if (IsSkillActive())
	{
		bool Return{ false };
		if (!getHeroStatsComp()->checkStamina(-(GETSUGA_COST)))
		{
			NotEnoughStamina();
			Return = true;
		}
		if (!getHeroStatsComp()->checkPower(-(GETSUGA_COST)))
		{
			NotEnoughPower();
			Return = true;
		}
		if (Return) { return; }

		GET_STATS->AddStamina(GETSUGA_COST, 0.f, true);
		SpawnHelper("sh_GetsugaFWHelper", 0.f, FRotator(45.f, 0.f, 0.f));
		SkillDisable();
	}
}

void AIchigo::sh_SwordTwist()
{
	if (!getHeroStatsComp()->CheckSkill("SwordTwist"))
		return;

	FState nState;
	nState.State = EIchigoState::Ichi_SwordTwist;
	nState.Animation = "SwordTwist";
	nState.EndState = false;
	NewState(nState);

	SpawnHelper("sh_SwordTwist", getFrameTime(4));
	SetBlockingAttack(EBlockType::Both, AnimElemTime(3), AnimElemTime(10));
	Combo(getFrameTime(10));

	FTimerHandle timer;
	SET_TIMER(timer, this, &AIchigo::sh_SwordTwistLoop, getFrameTime(10));
	SkillDisable();
}

void AIchigo::sh_SwordTwistLoop()
{
	if (CheckState(EIchigoState::Ichi_SwordTwist))
	{
		FState nState;
		nState.State = EIchigoState::Ichi_SwordTwistLoop;
		nState.Animation = "SwordTwistLoop";
		nState.EndState = false;
		NewState(nState);

		SET_TIMER(sh_STwistEndTimer, this, &AIchigo::sh_SwordTwistEnd, cTime(1.f));
		Combo(getFrameTime(3));
	}
}

void AIchigo::sh_SwordTwistEnd()
{
	if (CheckState(EIchigoState::Ichi_SwordTwistLoop))
	{
		FState nState;
		nState.State = EIchigoState::Ichi_SwordTwistEnd;
		nState.Animation = "SwordTwist";
		nState.AnimationFrame = 11;
		NewState(nState);
	}
}

void AIchigo::sh_SwordThrow()
{
	if (!getHeroStatsComp()->CheckSkill("SwordThrow"))
		return;

	ResetKeys();

	FState nState;
	nState.State = EIchigoState::Ichi_SwordThrow;
	nState.Animation = "SwordThrow";
	NewState(nState);
	Combo(getFrameTime(16));

	// Stamina
	GET_STATS->AddStamina(
		-1.f / getHeroStatsComp()->getTeleportCost(),
		getFrameTime(5),
		false,
		EIchigoState::Ichi_SwordThrow
	);

	AddImpulse(MoveVector * 300, getFrameTime(2));
	SpawnHelper("sh_SwordThrow", getFrameTime(5));

	SetBlockingAttack(EBlockType::Forward, getFrameTime(5), BLOCK_DURATION);
}

void AIchigo::sh_GetsugaStart()
{
	bool Return{ false };
	if (!getHeroStatsComp()->checkStamina(-(GETSUGA_TENSHOU_COST)))
	{
		NotEnoughStamina();
		Return = true;
	}

	if (!getHeroStatsComp()->checkPower(-(GETSUGA_TENSHOU_COST)))
	{
		NotEnoughPower();
		Return = true;
	}
	if (Return)
	{
		return;
	}

	FState nState;
	nState.State = EIchigoState::Ichi_GetsugaStart;
	nState.Animation = "GetsugaStart";
	NewState(nState);

	SpawnHelper("sh_GetsugaExplosion", getFrameTime(2));
	SpawnHelper("sh_GetsugaExplosion", getFrameTime(5));
	SpawnHelper("sh_GetsugaExplosion", getFrameTime(8));
	SetImmortality(getFrameTime(15));

	Combo(getFrameTime(10));
	SkillDisable();
}

void AIchigo::sh_GetsugaSlash()
{
	FState nState;
	nState.State = EIchigoState::Ichi_GetsugaFW;
	nState.Animation = "GetsugaFW";
	NewState(nState);

	SpawnHelper("sh_GetsugaHelper", getFrameTime(3));
	GET_STATS->AddStamina(GETSUGA_TENSHOU_COST, getFrameTime(2), true, GetState());

	DangerN(getFrameTime(5), EDangerType::MeleeAttack);
}

void AIchigo::sh_RExplosion()
{
	if (!getHeroStatsComp()->CheckSkill("RExplosion"))
		return;

	bool Return{ false };
	if (!getHeroStatsComp()->checkStamina(-(EXPLOSION_COST)))
	{
		NotEnoughStamina();
		Return = true;
	}
	if (!getHeroStatsComp()->checkPower(-(EXPLOSION_COST)))
	{
		NotEnoughPower();
		Return = true;
	}
	if (Return) { return; }

	FState nState;
	nState.State = EIchigoState::Ichi_RExplosion;
	nState.Animation = "RExplosion";
	nState.Rotate = false;
	NewState(nState);

	SpawnHelper("sh_ReiatsuExplosion", getFrameTime(8), FRotator(0.f), FVector(1.5f));
	Combo(getFrameTime(18));

	DangerN(getFrameTime(5), EDangerType::MeleeAttack);
	SetBlockingAttack(EBlockType::Both, AnimElemTime(5), AnimElemTime(15));

	GET_STATS->AddStamina(EXPLOSION_COST, AnimElemTime(10), true, GetState());
	SkillDisable();

}

void AIchigo::sh_Bankai()
{
	if (!getHeroStatsComp()->CheckSkill("Bankai"))
		return;

	FState nState;
	nState.State = EIchigoState::Ichi_Bankai;

	if (getHeroStatsComp()->CheckSkill("FastBankai"))
	{
		nState.Animation = "FastBankai";
		NewState(nState);

		SpawnHelper("BankaiEff", getFrameTime(6));
		SpawnHelper("sh_ReiatsuExplosion", getFrameTime(10), FRotator(0.f), FVector(2.f));
		SetImmortality(AnimElemTime(18));
		Combo(getFrameTime(18));
	}
	else
	{
		nState.Animation = "Bankai";
		NewState(nState);

		SpawnHelper("BankaiEff", getFrameTime(6));
		SpawnHelper("BankaiEff", getFrameTime(12));
		SpawnHelper("BankaiEff", getFrameTime(18));
		SpawnHelper("BankaiEff", getFrameTime(24));
		SpawnHelper("sh_ReiatsuExplosion", getFrameTime(28), FRotator(0.f), FVector(2.f));
		SetImmortality(AnimElemTime(35));
		Combo(getFrameTime(32));
	}

	Bankai();
	IsUsedBankai = true;

	GET_STATS->SetExpMultiplier(0.5f);
	GET_STATS->AddStamina(2.f);
	if (!GET_STATS->checkPower(-(BANKAI_COST)))
	{
		float hpReducer = (GET_STATS->getPower() + BANKAI_COST) * 0.5f;
		GET_STATS->AddHealth(hpReducer);
	}
	GET_STATS->AddPower(-3.f);
	SkillDisable();
}

// End Action /---------------------------------/

//----------------------------------------------/ Combo

void AIchigo::ShikaiComboI()
{
	EComboKey key = getNextKey();

	switch (GetState())
	{
	case EIchigoState::Ichi_Attack_1:
	{
		switch (key)
		{
		case EComboKey::CK_Attack:			{ sh_Attack_2(); break; }
		case EComboKey::CK_Dash:			{ DoDash();		 break; }
		}
		break;
	}
	case EIchigoState::Ichi_Attack_2:
	{
		switch (key)
		{
		case EComboKey::CK_AForward:		{ sh_Attack_FW(); break; }
		case EComboKey::CK_ABackward:		{ sh_Attack_B(); break; }
		case EComboKey::CK_Dash:			{ DoDash(); break; }
		}
		break;
	}

	case EIchigoState::Ichi_Attack_B:
	{
		switch (key)
		{
		case EComboKey::CK_Dash:			{ DoDash(); break; }
		}
		break;
	}
	case EIchigoState::Ichi_Attack_FW:
	{
		switch (key)
		{
		case EComboKey::CK_Dash:			{ DoDash(); break; }
		}
		break;
	}
	case EIchigoState::Ichi_SwordTwist:
	{
		switch (key)
		{
		case EComboKey::CK_AForward:		{ sh_SwordThrow(); break; }
		case EComboKey::CK_ABackward:		{ sh_Attack_B(); break; }
		}
		break;
	}
	case EIchigoState::Ichi_SwordTwistLoop:
	{
		switch (key)
		{
		case EComboKey::CK_AForward:		{ sh_SwordThrow(); break; }
		case EComboKey::CK_ABackward:		{ sh_Attack_B(); break; }
		}
		break;
	}
	case EIchigoState::Ichi_SwordThrow:
	{
		switch (key)
		{
		case EComboKey::CK_Dash:			{ DoDash(); break; }
		}
		break;
	}
	case EIchigoState::Ichi_GetsugaStart:	{ sh_GetsugaSlash(); break; }
	} // End Switch
}

// End Combo /----------------------------------/