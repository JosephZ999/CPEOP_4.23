// Fill out your copyright notice in the Description page of Project Settings.


#include "Chars/Heroes/Ichigo.h"
#include "TimerManager.h"

#include "PaperFlipbook.h"

#define ANIM_LOC		"Texture/Chars/Ichigo/FBook/"
#define ANIM_LOC_B		"Texture/Chars/Ichigo_Bankai/FBook/"
#define HIT_LOC			"Blueprint/Chars/Ichigo/Shikai/"
#define HIT_LOC_B		"Blueprint/Chars/Ichigo/Bankai/"

// Attack Options
#define BASE_VELOCITY	(MoveVector + GetActorForwardVector()) * 150
#define SP_VELOCITY		(MoveVector + GetActorForwardVector()) * 200

#define BLOCK_DURATION cTime(0.2f)

// Skills list
/*
	Getsuga
	SwordTwist
	RExplosion
*/


AIchigo::AIchigo()
{
	if (getHeroStatsComp())
	{
		InitForm(SHIKAI_NAME, { 4, 3, 3 }); // 10
		InitForm(BANKAI_NAME, { 5, 8, 4 });
	}

	// Hit Boxes
	InitHelper("sh_Attack_1",			HIT_LOC "sh_Attack_1");
	InitHelper("sh_Attack_2",			HIT_LOC "sh_Attack_2");
	InitHelper("sh_AttackBack",			HIT_LOC "sh_AttackBack");
	InitHelper("sh_AttackForward",		HIT_LOC "sh_AttackForward");
	InitHelper("sh_GetsugaHelper",		HIT_LOC "sh_GetsugaHelper");
	InitHelper("sh_GetsugaFWHelper",	HIT_LOC "sh_GetsugaFWHelper");
	InitHelper("sh_ReiatsuExplosion",	HIT_LOC "sh_ReiatsuExplosion");
	InitHelper("sh_SwordTwist",			HIT_LOC "sh_SwordTwist");

	InitHelper("b_Attack_1",			HIT_LOC_B "b_Attack_1");
	InitHelper("b_Attack_2",			HIT_LOC_B "b_Attack_2");
	InitHelper("b_Attack_B",			HIT_LOC_B "b_Attack_B");


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
	AddAnimation("SwordTwist",		ANIM_LOC "SwordTwist");
	AddAnimation("SwordTwistLoop",	ANIM_LOC "SwordTwistLoop");
	AddAnimation("GetsugaStart",	ANIM_LOC "GetsugaStart");
	AddAnimation("GetsugaFW",		ANIM_LOC "GetsugaFW");
	AddAnimation("RExplosion",		ANIM_LOC "RExplosion");


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

	AddAnimation("Attack_1",        ANIM_LOC_B "Attack1");
	AddAnimation("Attack_2",        ANIM_LOC_B "Attack2");

	AddAnimation("Attack_B",        ANIM_LOC_B "AttackB");


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
			case (uint8)EIchigoShikai::Attack_1: 
			{ 
				if (isComboTime()) { sh_Attack_2(); resetKeys(); } 
				break; 
			}
			case (uint8)EIchigoShikai::AttackFW:
			{
				SkillEnable();
				UE_LOG(LogTemp, Warning, TEXT("Activate this fucking skill"));
				break;
			}
			case (uint8)EIchigoShikai::AttackB:
			{
				SkillEnable();
				break;
			}

			case (uint8)EIchigoShikai::SwordTwistLoop:
			{
				SET_TIMER(sh_STwistEndTimer, this, &AIchigo::sh_SwordTwistEnd, cTime(1.f));
				GET_STATS->AddStamina(0.07);
				break;
			}

			} // End Switch
			return;
		}
	
		if (getHeroStatsComp()->FormName == BANKAI_NAME)
		{
			switch (getState())
			{
			case EBaseStates::Stand: { b_Attack_1(); break; }
			case (uint8)EIchigoBankai::Attack_1:
			{
				if (isComboTime()) { b_Attack_2(); resetKeys(); }
				break;
			}
			} // Switch End
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
				if (SkillisActive())
				{
					sh_SwordTwist(); SkillDisable();
				}
				else
				{
					sh_AttackB();
				}
				break;
			}
			case EBaseStates::Jumping: { break; }
			case (uint8)EIchigoShikai::Attack_2: { if (isComboTime()) { sh_AttackB(); resetKeys(); } break; }
			case EBaseStates::PowChargeLoop: { sh_SwordTwist(); SkillDisable(); break; }
			case (uint8)EIchigoShikai::SwordTwistLoop:
			{
				sh_RExplosion(); break;
			}

			} // End Switch
		}

		if (getHeroStatsComp()->FormName == BANKAI_NAME)
		{
			switch (getState())
			{
			case EBaseStates::Stand: { b_Attack_B(); break; }
			case (uint8)EIchigoBankai::Attack_2:
			{
				if (isComboTime())
				{
					b_Attack_B(); resetKeys();
				}
				break;
			}

			} // Switch End
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
				if (SkillisActive())
				{
					sh_GetsugaStart();
				}
				else
				{
					sh_AttackFW();
				}
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

		FTimerHandle nTimer;
		SET_TIMER(nTimer, this, &AIchigo::sh_GetsugaFW, getFrameTime(5));
	}

	void AIchigo::sh_GetsugaFW()
	{
		if (!checkState((uint8)EIchigoShikai::AttackFW))
			return;

		if (!getHeroStatsComp()->CheckSkill("Getsuga"))
			return;

		if (SkillisActive()
			&& getHeroStatsComp()->checkStamina(-(GETSUGA_COST))
			&& getHeroStatsComp()->checkPower(-(GETSUGA_COST)))
		{
			GET_STATS->AddStamina(GETSUGA_COST, 0.f, true);
			SpawnHelper("sh_GetsugaFWHelper");
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


		FTimerHandle nTimer;
		SET_TIMER(nTimer, this, &AIchigo::sh_GetsugaB, getFrameTime(6));
	}

	void AIchigo::sh_GetsugaB()
	{
		if (!checkState((uint8)EIchigoShikai::AttackB))
			return;

		if (!getHeroStatsComp()->CheckSkill("Getsuga"))
			return;

		if (SkillisActive()
			&& getHeroStatsComp()->checkStamina(-(GETSUGA_COST))
			&& getHeroStatsComp()->checkPower(-(GETSUGA_COST)))
		{
			GET_STATS->AddStamina(GETSUGA_COST, 0.f, true);
			SpawnHelper("sh_GetsugaFWHelper", 0.f, FRotator(45.f, 0.f, 0.f));
			SkillDisable();
		}
	}

	//---------------------------------------------// Sword Twist
	void AIchigo::sh_SwordTwist()
	{
		if (!getHeroStatsComp()->CheckSkill("SwordTwist"))
			return;

		NewState((uint8)EIchigoShikai::SwordTwist, "SwordTwist", 0, false, false);
		SetRotation(isMovingRight());
		SpawnHelper("sh_SwordTwist", getFrameTime(4));
		SetBlockingAttack(EBlockType::Both, AnimElemTime(4), AnimElemTime(10));

		FTimerHandle timer;
		GetWorldTimerManager().SetTimer(timer, this, &AIchigo::sh_SwordTwistLoop, getFrameTime(10));
	}

	void AIchigo::sh_SwordTwistLoop()
	{
		if (getState() == (uint8)EIchigoShikai::SwordTwist)
		{
			NewState((uint8)EIchigoShikai::SwordTwistLoop, "SwordTwistLoop", 0, false, false);
			GetWorldTimerManager().SetTimer(sh_STwistEndTimer, this, &AIchigo::sh_SwordTwistEnd, cTime(1.f));
			Combo(getFrameTime(3));
		}
	}

	void AIchigo::sh_SwordTwistEnd()
	{
		if (getState() == (uint8)EIchigoShikai::SwordTwistLoop)
		{
			NewState((uint8)EIchigoShikai::SwordTwistEnd, "SwordTwist", 11);
		}
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

	void AIchigo::sh_GetsugaSlash()
	{
		NewState		((uint8)EIchigoShikai::GetsugaFW, "GetsugaFW");
		SetRotation		(isMovingRight());
		SpawnHelper		("sh_GetsugaHelper", getFrameTime(3));
		GET_STATS->AddStamina(GETSUGA_TENSHOU_COST, getFrameTime(2), true, getState());

		SetBlockingAttack
			(EBlockType::Both, getFrameTime(0), BLOCK_DURATION);
		DangerN
			(getFrameTime(5), EDangerType::MeleeAttack);
	}


	//---------------------------------------------// Reiatsu Explosion
	void AIchigo::sh_RExplosion()
	{
		if (!getHeroStatsComp()->CheckSkill("RExplosion"))
			return;

		if (getHeroStatsComp()->checkStamina(-(EXPLOSION_COST)) && getHeroStatsComp()->checkPower(-(EXPLOSION_COST)))
		{
			NewState((uint8)EIchigoShikai::RExplosion, "RExplosion");
			SpawnHelper("sh_ReiatsuExplosion", getFrameTime(8), FRotator(0.f), FVector(1.5f));
			Combo(getFrameTime(18));

			DangerN(getFrameTime(5), EDangerType::MeleeAttack);
			SetBlockingAttack(EBlockType::Both, AnimElemTime(5), AnimElemTime(15));

			GET_STATS->AddStamina(EXPLOSION_COST, AnimElemTime(10), true, getState());
		}
	}

	// Bankai

	void AIchigo::sh_Bankai()
	{
		if (!getHeroStatsComp()->CheckSkill("Bankai"))
			return;

		NewState((uint8)EIchigoShikai::Bankai, "Bankai");
		SetImmortality(AnimElemTime(35));
		Bankai();
	}

//---------------------------------------------// Bankai Actions //////////////////////////////////
	void AIchigo::b_Attack_1()
	{
		NewState((uint8)EIchigoBankai::Attack_1, "Attack_1");
		SetRotation(isMovingRight());
		AddImpulse(BASE_VELOCITY, getFrameTime(1));
		SpawnHelper("b_Attack_1", getFrameTime(3));
		Combo(getFrameTime(7));

		SetBlockingAttack(EBlockType::Forward, getFrameTime(3), BLOCK_DURATION);
		DangerN(getFrameTime(6), EDangerType::MeleeAttack);
	}

	void AIchigo::b_Attack_2()
	{
		NewState((uint8)EIchigoBankai::Attack_2, "Attack_2");
		SetRotation(isMovingRight());
		AddImpulse(BASE_VELOCITY, getFrameTime(1));
		SpawnHelper("b_Attack_2", getFrameTime(3));
		Combo(getFrameTime(7));

		SetBlockingAttack(EBlockType::Forward, getFrameTime(3), BLOCK_DURATION);
		DangerN(getFrameTime(6), EDangerType::MeleeAttack);
	}

	void AIchigo::b_Attack_3()
	{
	}

	void AIchigo::b_Attack_FW()
	{
	}

	void AIchigo::b_Attack_B()
	{
		NewState((uint8)EIchigoBankai::Attack_B, "Attack_B");
		SetRotation(isMovingRight());
		AddImpulse(SP_VELOCITY, getFrameTime(4));
		SpawnHelper("b_Attack_B", getFrameTime(6));
		Combo(getFrameTime(11));

		SetBlockingAttack(EBlockType::Forward, getFrameTime(7), BLOCK_DURATION);
		DangerN(getFrameTime(6), EDangerType::MeleeAttack);
	}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Combo Implementation /--------------------------------------------------------------------------
	void AIchigo::ComboI()
	{
		Super::ComboI();

		FName form = getHeroStatsComp()->FormName;

		if		(form == SHIKAI_NAME)	{ ShikaiComboI(); }
		else if (form == BANKAI_NAME)	{ BankaiComboI(); }
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
			case EComboKey::CK_Attack:	{ sh_Attack_2(); break; }
			case EComboKey::CK_Dash:	{ DoDash();		 break; }
			}
			break;
		}
		case (uint8)EIchigoShikai::Attack_2:
		{
			switch (key)
			{
			case EComboKey::CK_AForward:	{ sh_AttackFW(); break;}
			case EComboKey::CK_ABackward:	{ sh_AttackB();	 break; }
			case EComboKey::CK_Dash:		{ DoDash();		 break; }
			}
			break;
		}
		case (uint8)EIchigoShikai::GetsugaStart:
		{
			sh_GetsugaSlash();
			break;
		}
		} // End Switch
	}

	void AIchigo::BankaiComboI()
	{
		EComboKey key = getNextKey();

		switch (getState())
		{

		case (uint8)EIchigoBankai::Attack_1:
		{
			switch (key)
			{
			case EComboKey::CK_Attack: { b_Attack_2(); break; }
			case EComboKey::CK_Dash:   { DoDash();     break; }
			}
			break;
		}
		case (uint8)EIchigoBankai::Attack_2:
		{
			switch (key)
			{
			// case EComboKey::CK_Attack: { b_Attack_2(); break; }
			case EComboKey::CK_ABackward: { b_Attack_B(); break; }
			case EComboKey::CK_Dash:      { DoDash();     break; }
			} // Switch End
			break;
		}

		} // Switch End
	}