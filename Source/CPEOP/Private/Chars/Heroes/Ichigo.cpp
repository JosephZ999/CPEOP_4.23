// Fill out your copyright notice in the Description page of Project Settings.


#include "Chars/Heroes/Ichigo.h"
#include "TimerManager.h"

#include "PaperFlipbook.h"

AIchigo::AIchigo()
{
	if (getHeroStatsComp())
	{
		InitForm(SHIKAI_NAME, { 4, 3, 3 }); // 10
		InitForm(BANKAI_NAME, { 5, 8, 4 });
	}

	// Hit Boxes
	InitHelper("sh_Attack_1", HIT_LOC "sh_Attack_1");
	InitHelper("sh_Attack_2", HIT_LOC "sh_Attack_2");
	InitHelper("sh_AttackBack", HIT_LOC "sh_AttackBack");
	InitHelper("sh_AttackForward", HIT_LOC "sh_AttackForward");

	InitHelper("sh_GetsugaHelper", HIT_LOC "sh_GetsugaHelper");
	InitHelper("sh_GetsugaFWHelper", HIT_LOC "sh_GetsugaFWHelper");

// Animations //
	// Shikai
	AnimData = &ShikaiAnim;
	AddAnimation("Stand",			ANIM_LOC "Stand"		);
	AddAnimation("Walk",			ANIM_LOC "Run"			);
	AddAnimation("JumpStart",		ANIM_LOC "JumpStart"	);
	AddAnimation("JumpUp",			ANIM_LOC "JumpUp"		);
	AddAnimation("JumpHold",		ANIM_LOC "JumpHold"		);
	AddAnimation("JumpDown",		ANIM_LOC "JumpDown"		);
	AddAnimation("JumpLand",		ANIM_LOC "JumpLand"		);

	AddAnimation("Hit",				ANIM_LOC "hit"			);
	AddAnimation("FallHold",		ANIM_LOC "FallHold"		);
	AddAnimation("FallUp",			ANIM_LOC "FallUp"		);
	AddAnimation("FallDown",		ANIM_LOC "FallDown"		);
	AddAnimation("StandUp",			ANIM_LOC "StandUp"		);
	AddAnimation("StandUpAir",		ANIM_LOC "StandUpAir"	);

	AddAnimation("Block",			ANIM_LOC "Guard"		);
	AddAnimation("BlockAir",		ANIM_LOC "GuardAir"		);

	AddAnimation("PowChargeStart",	ANIM_LOC "PowChStart"	);
	AddAnimation("PowChargeLoop",	ANIM_LOC "PowChLoop"	);
	AddAnimation("PowChargeEnd",	ANIM_LOC "PowChEnd"		);

	AddAnimation("Attack_1",		ANIM_LOC "Attack1");
	AddAnimation("Attack_2",		ANIM_LOC "Attack2");
	AddAnimation("AttackFW",		ANIM_LOC "AttackForward");
	AddAnimation("AttackB",			ANIM_LOC "AttackBack");

	AddAnimation("GetsugaStart",	ANIM_LOC "GetsugaStart");
	AddAnimation("GetsugaFW",		ANIM_LOC "GetsugaFW");
	AddAnimation("Bankai",			ANIM_LOC "Bankai");

	AnimData = &BankaiAnim;
	AddAnimation("Stand",           ANIM_LOC_B "Stand"	);
	AddAnimation("Walk",            ANIM_LOC_B "Run"		);
	AddAnimation("JumpStart",       ANIM_LOC_B "JumpStart");
	AddAnimation("JumpUp",          ANIM_LOC_B "JumpUp"	);
	AddAnimation("JumpHold",        ANIM_LOC_B "JumpHold"	);
	AddAnimation("JumpDown",        ANIM_LOC_B "JumpDown"	);
	AddAnimation("JumpLand",        ANIM_LOC_B "JumpLand"	);

	AddAnimation("Hit",             ANIM_LOC_B "Hit");
	AddAnimation("FallHold",        ANIM_LOC_B "FallHold");
	AddAnimation("FallUp",          ANIM_LOC_B "FallUp");
	AddAnimation("FallDown",        ANIM_LOC_B "FallDown");
	AddAnimation("StandUp",         ANIM_LOC_B "StandUp");
	AddAnimation("StandUpAir",      ANIM_LOC_B "StandUpAir");

	AddAnimation("Block",           ANIM_LOC_B "Guard");
	AddAnimation("BlockAir",        ANIM_LOC_B "Guard");

	AddAnimation("PowChargeStart",  ANIM_LOC_B "ChargeStart");
	AddAnimation("PowChargeLoop",   ANIM_LOC_B "ChargeLoop");
	AddAnimation("PowChargeEnd",    ANIM_LOC_B "ChargeEnd");

	AnimData = &ShikaiAnim;
}

void AIchigo::BeginPlay()
{
	Super::BeginPlay();
	ChangeForm(SHIKAI_NAME);
}


//---------------------------------------------// Inputs
	void AIchigo::Attack()
	{
		Super::Attack();

		if (getHeroStatsComp()->FormName == SHIKAI_NAME)
		{
			switch (getState())
			{
			case EBaseStates::Stand: { sh_Attack_1(); break; }
			case EBaseStates::Jumping: { break; }

			case (uint8)EIchigoShikai::Attack_1: { if (isComboTime()) { sh_Attack_2(); resetKeys(); } break; }

			} // End Switch

		}
	}
	void AIchigo::AttackBack()
	{
		Super::AttackBack();

		if (getHeroStatsComp()->FormName == SHIKAI_NAME)
		{
			switch (getState())
			{
			case EBaseStates::Stand:
			{
				sh_AttackB();
				break;
			}
			case EBaseStates::Jumping: { break; }
			case (uint8)EIchigoShikai::Attack_2: { if (isComboTime()) { sh_AttackB(); resetKeys(); } break; }
			} // End Switch
		}
	}
	void AIchigo::AttackForward()
	{
		Super::AttackForward();

		if (getHeroStatsComp()->FormName == SHIKAI_NAME)
		{
			switch (getState())
			{
			case EBaseStates::Stand:
			{
				sh_AttackFW();
				break;
			}
			case EBaseStates::Jumping: { break; }
			case (uint8)EIchigoShikai::Attack_2: { if (isComboTime()) { sh_AttackFW(); resetKeys(); } break; }
			case EBaseStates::PowChargeLoop: { sh_GetsugaStart(); break; }
			} // End Switch
		}
	}
	void AIchigo::Btn_Bankai()
	{
		if (getHeroStatsComp()->FormName == SHIKAI_NAME)
		{
			switch (getState())
			{
			case EBaseStates::Stand: { if (SkillisActive()) { sh_Bankai(); } break; }
			case EBaseStates::PowChargeLoop: { sh_Bankai(); break; }
			}
		}
	}

//---------------------------------------------// Shikai Actions //////////////////////////////////
//---------------------------------------------// Attacks
	void AIchigo::sh_Attack_1()
	{
		NewState		((uint8)EIchigoShikai::Attack_1, "Attack_1");
		SetRotation		(isMovingRight());
		AddImpulse		(BASE_VELOCITY, getFrameTime(2));
		SpawnHelper		("sh_Attack_1", getFrameTime(4));
		Combo			(getFrameTime(9));

		SetBlockingAttack(EBlockType::Forward, getFrameTime(4), BLOCK_DURATION);
		DangerN(getFrameTime(6), EDangerType::MeleeAttack);

	}
	void AIchigo::sh_Attack_2()
	{
		NewState		((uint8)EIchigoShikai::Attack_2, "Attack_2");
		SetRotation		(isMovingRight());
		AddImpulse		(BASE_VELOCITY, getFrameTime(1));
		SpawnHelper		("sh_Attack_2", getFrameTime(4));
		Combo			(getFrameTime(9));

		SetBlockingAttack(EBlockType::Forward, getFrameTime(4), BLOCK_DURATION);
		DangerN(getFrameTime(6), EDangerType::MeleeAttack);
	}
	void AIchigo::sh_AttackFW()
	{
		NewState		((uint8)EIchigoShikai::AttackFW, "AttackFW");
		SetRotation		(isMovingRight());
		AddImpulse		(SP_VELOCITY, getFrameTime(1));
		SpawnHelper		("sh_AttackForward", getFrameTime(5), FRotator(20.f, 0.f, 0.f));

		SetBlockingAttack(EBlockType::Forward, getFrameTime(5), BLOCK_DURATION);
		DangerN(getFrameTime(7), EDangerType::MeleeAttack);

		if (SkillisActive() 
			&& getHeroStatsComp()->checkStamina(-(GETSUGA_FW_COST))
			&& getHeroStatsComp()->checkPower(-(GETSUGA_FW_COST)))
		{
			GET_STATS->AddStamina(GETSUGA_FW_COST, getFrameTime(5), true);
			SpawnHelper("sh_GetsugaFWHelper", getFrameTime(5));
			SkillDisable();
		}
	}
	void AIchigo::sh_AttackB()
	{
		NewState		((uint8)EIchigoShikai::AttackB, "AttackB");
		SetRotation		(isMovingRight());
		AddImpulse		(SP_VELOCITY, getFrameTime(4));
		SpawnHelper		("sh_AttackBack", getFrameTime(6), FRotator(20.f, 0.f, 0.f));

		SetBlockingAttack(EBlockType::Forward, getFrameTime(6), BLOCK_DURATION);
		DangerN(getFrameTime(8), EDangerType::MeleeAttack);
	}

	//---------------------------------------------// Getsuga Tensho
	void AIchigo::sh_GetsugaStart()
	{
		if (getHeroStatsComp()->checkStamina(-(GETSUGA_TENSHOU_COST))
			&& getHeroStatsComp()->checkPower(-(GETSUGA_TENSHOU_COST)))
		{
			NewState		((uint8)EIchigoShikai::GetsugaStart, "GetsugaStart");
			SetRotation		(isMovingRight());
			Combo			(getFrameTime(10));
		}
	}

	void AIchigo::sh_GetsugaFW()
	{
		NewState		((uint8)EIchigoShikai::GetsugaFW, "GetsugaFW");
		SetRotation		(isMovingRight());
		SpawnHelper		("sh_GetsugaHelper", getFrameTime(3));
		GET_STATS->AddStamina(GETSUGA_TENSHOU_COST, getFrameTime(2), true);

		SetBlockingAttack
			(EBlockType::Both, getFrameTime(0), BLOCK_DURATION);
		DangerN
			(getFrameTime(5), EDangerType::MeleeAttack);
	}

	void AIchigo::sh_Bankai()
	{
		NewState((uint8)EIchigoShikai::Bankai, "Bankai");
		SetImmortality(AnimElemTime(30));
		Bankai();
	}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Combo Implementation /--------------------------------------------------------------------------
	void AIchigo::ComboI()
	{
		Super::ComboI();

		FName form = getHeroStatsComp()->FormName;

		if		(form == SHIKAI_NAME)	{ ShikaiComboI(); }
		else if (form == BANKAI_NAME)	{ BankaiComboI(); }
		
		else
		{

		}
	}

	void AIchigo::ShikaiComboI()
	{
		EComboKey key = getNextKey();

		switch (getState())
		{

		case (uint8)EIchigoShikai::Attack_1: 
		{ 
			switch (key)
			{
			case EComboKey::CK_Attack:	{ sh_Attack_2();	break; }
			case EComboKey::CK_Dash:	{ DoDash();			break; }
			}
		}
		case (uint8)EIchigoShikai::Attack_2:
		{
			switch (key)
			{
			case EComboKey::CK_AForward:	{ sh_AttackFW();	break; }
			case EComboKey::CK_ABackward:	{ sh_AttackB();		break; }
			case EComboKey::CK_Dash:		{ DoDash();			break; }
			}
			break;
		}
		case (uint8)EIchigoShikai::GetsugaStart:
		{
			sh_GetsugaFW();
			break;
		}
		} // End Switch
	}

	void AIchigo::BankaiComboI()
	{

	}