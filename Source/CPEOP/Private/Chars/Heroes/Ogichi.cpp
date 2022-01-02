// Fill out your copyright notice in the Description page of Project Settings.

#include "Chars/Heroes/Ogichi.h"
#include "TimerManager.h"
#include "PaperFlipbook.h"

AOgichi::AOgichi()
{
	if (getHeroStatsComp())
	{
		InitForm(SHIKAI_NAME, { 5, 2, 10 });
		InitForm(BANKAI_NAME, { 5, 12, 10 });
	}

	// Hit Boxes
	InitHelper("sh_Attack_1",			HIT_LOC "sh_Attack_1");
	InitHelper("sh_Attack_2",			HIT_LOC "sh_Attack_2");
	InitHelper("sh_AttackBack",			HIT_LOC "sh_AttackBack");
	InitHelper("sh_AttackForward",		HIT_LOC "sh_AttackForward");
	InitHelper("sh_SwordTwist",			HIT_LOC "sh_SwordTwist");
	InitHelper("sh_SwordThrow",			HIT_LOC "sh_SwordThrow");

	InitHelper("sh_GetsugaHelper",		HIT_LOC "sh_GetsugaHelper");
	InitHelper("sh_GetsugaFWHelper",	HIT_LOC "sh_GetsugaFWHelper");

// Animations //
	// Shikai
	AnimData = &ShikaiAnim;
	AddAnimation("Stand",			ANIM_LOC "Stand");
	AddAnimation("Walk",			ANIM_LOC "Walk");
	AddAnimation("JumpStart",		ANIM_LOC "JumpStart");
	AddAnimation("JumpUp",			ANIM_LOC "JumpUp");
	AddAnimation("JumpHold",		ANIM_LOC "JumpHold");
	AddAnimation("JumpDown",		ANIM_LOC "JumpDown");
	AddAnimation("JumpLand",		ANIM_LOC "JumpLand");

	AddAnimation("Hit",				ANIM_LOC "Hit1");
	AddAnimation("FallHold",		ANIM_LOC "FallHold");
	AddAnimation("FallUp",			ANIM_LOC "FallUp");
	AddAnimation("FallDown",		ANIM_LOC "FallDown");
	AddAnimation("StandUp",			ANIM_LOC "StandUp");
	AddAnimation("StandUpAir",		ANIM_LOC "StandUpAir");

	AddAnimation("Block",			ANIM_LOC "Block");
	AddAnimation("BlockAir",		ANIM_LOC "BlockAir");

	AddAnimation("PowChargeStart",	ANIM_LOC "PowChStart");
	AddAnimation("PowChargeLoop",	ANIM_LOC "PowChLoop");
	AddAnimation("PowChargeEnd",	ANIM_LOC "PowChEnd");

	AddAnimation("Attack_1",		ANIM_LOC "Attack1");
	AddAnimation("Attack_2",		ANIM_LOC "Attack2");
	AddAnimation("AttackFW",		ANIM_LOC "AttackForward");
	AddAnimation("AttackB",			ANIM_LOC "AttackBack");
	AddAnimation("AttackB2",		ANIM_LOC "AttackBack2");
	AddAnimation("SwordTwist",		ANIM_LOC "SwordTwist");
	AddAnimation("SwordTwistLoop",	ANIM_LOC "SwordTwistLoop");
	AddAnimation("SwordThrow",		ANIM_LOC "SwordThrow");

	AddAnimation("Getsuga",			ANIM_LOC "Getsuga");

	AnimData = &ShikaiAnim;

}

void AOgichi::BeginPlay()
{
	Super::BeginPlay();
	ChangeForm(SHIKAI_NAME);
}

//---------------------------------------------// Inputs
void AOgichi::Attack()
{
	Super::Attack();

	if (getHeroStatsComp()->FormName == SHIKAI_NAME)
	{
		switch (getState())
		{
		case EBaseStates::Stand:		{ sh_Attack_1(); break; }
		case EBaseStates::Jumping:		{ break; }
		case (uint8)EOgichiShikai::SwordTwistLoop:		
		{ 
			SET_TIMER(sh_STwistEndTimer, this, &AOgichi::sh_SwordTwistEnd, cTime(1.f));
			GET_STATS->AddStamina(0.07);
			break; 
		}
		case (uint8)EOgichiShikai::Attack_1: { if (isComboTime()) { sh_Attack_2(); resetKeys(); } break; }
		
		} // End Switch

	}
}
void AOgichi::AttackBack()
{
	Super::AttackBack();

	if (getHeroStatsComp()->FormName == SHIKAI_NAME)
	{
		switch (getState())
		{
		case EBaseStates::Stand:
		{
			if (SkillisActive()) { sh_SwordTwist(); SkillDisable(); }
			else { sh_AttackB(); }
			break;
		}
		case EBaseStates::Jumping: { break; }

		case (uint8)EOgichiShikai::SwordTwistLoop:
		{
			if (SkillisActive()) { SkillDisable(); }
			else { sh_AttackB2(); }
			break;
		}
		case (uint8)EOgichiShikai::Attack_2: { if (isComboTime()) { sh_SwordTwist(); resetKeys(); } break; }
		case EBaseStates::PowChargeLoop: { sh_SwordTwist(); SkillDisable(); break; }

		} // End Switch
	}
}
void AOgichi::AttackForward()
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
		case EBaseStates::Jumping:	{ break; }
		case (uint8)EOgichiShikai::SwordTwistLoop: 
		{ 
			if (SkillisActive())	{ sh_Getsuga(); SkillDisable(); }
			else					{ sh_SwordThrow(); }
			break;
		}
		case (uint8)EOgichiShikai::Attack_2: { if (isComboTime()) { sh_AttackFW(); resetKeys(); } break; }
		case EBaseStates::PowChargeLoop: { sh_Getsuga(); break; }
		} // End Switch
	}
}

//---------------------------------------------// Shikai Actions //////////////////////////////////
//---------------------------------------------// Attacks
void AOgichi::sh_Attack_1()
{
	NewState		((uint8)EOgichiShikai::Attack_1, "Attack_1");
	SetRotation		(isMovingRight());
	AddImpulse		(BASE_VELOCITY, getFrameTime(2));
	SpawnHelper		("sh_Attack_1", getFrameTime(4));
	Combo			(getFrameTime(9));

	SetBlockingAttack(EBlockType::Forward, getFrameTime(4), BLOCK_DURATION);
}
void AOgichi::sh_Attack_2()
{
	NewState		((uint8)EOgichiShikai::Attack_2, "Attack_2");
	SetRotation		(isMovingRight());
	AddImpulse		(BASE_VELOCITY, getFrameTime(1));
	SpawnHelper		("sh_Attack_2", getFrameTime(4));
	Combo			(getFrameTime(9));

	SetBlockingAttack(EBlockType::Forward, getFrameTime(4), BLOCK_DURATION);
}
void AOgichi::sh_AttackFW()
{
	NewState		((uint8)EOgichiShikai::AttackFW, "AttackFW");
	SetRotation		(isMovingRight());
	AddImpulse		(SP_VELOCITY, getFrameTime(1));
	SpawnHelper		("sh_AttackForward", getFrameTime(5), FRotator(20.f, 0.f, 0.f));

	SetBlockingAttack(EBlockType::Forward, getFrameTime(5), BLOCK_DURATION);

	if (SkillisActive()
		&& getHeroStatsComp()->checkStamina(-(GETSUGA_FW_COST))
		&& getHeroStatsComp()->checkPower(-(GETSUGA_FW_COST)))
	{
		GET_STATS->AddStamina(GETSUGA_FW_COST, getFrameTime(5), true);
		SpawnHelper("sh_GetsugaFWHelper", getFrameTime(5));
		SkillDisable();
	}
}
void AOgichi::sh_AttackB()
{
	NewState		((uint8)EOgichiShikai::AttackB, "AttackB");
	SetRotation		(isMovingRight());
	AddImpulse		(SP_VELOCITY, getFrameTime(4));
	SpawnHelper		("sh_AttackBack", getFrameTime(6), FRotator(20.f, 0.f, 0.f));

	SetBlockingAttack(EBlockType::Forward, getFrameTime(6), BLOCK_DURATION);
}
void AOgichi::sh_AttackB2()
{
	NewState		((uint8)EOgichiShikai::AttackB, "AttackB2");
	SetRotation		(isMovingRight());
	AddImpulse		(SP_VELOCITY, getFrameTime(7));
	SpawnHelper		("sh_AttackBack", getFrameTime(9), FRotator(20.f, 0.f, 0.f));

	SetBlockingAttack(EBlockType::Forward, getFrameTime(9), BLOCK_DURATION);
}

//---------------------------------------------// Sword Twist
void AOgichi::sh_SwordTwist()
{
	NewState		((uint8)EOgichiShikai::SwordTwist, "SwordTwist", 0, false, false);
	SetRotation		(isMovingRight());
	SpawnHelper		("sh_SwordTwist", getFrameTime(4));

	FTimerHandle timer;
	GetWorldTimerManager().SetTimer(timer, this, &AOgichi::sh_SwordTwistLoop, getFrameTime(10));
	
}
void AOgichi::sh_SwordTwistLoop()
{
	if (getState() == (uint8)EOgichiShikai::SwordTwist)
	{
		NewState((uint8)EOgichiShikai::SwordTwistLoop, "SwordTwistLoop", 0, false, false);
		GetWorldTimerManager().SetTimer(sh_STwistEndTimer, this, &AOgichi::sh_SwordTwistEnd, cTime(1.f));
		Combo(getFrameTime(3));
	}
}
void AOgichi::sh_SwordTwistEnd()
{
	if (getState() == (uint8)EOgichiShikai::SwordTwistLoop)
	{
		NewState((uint8)EOgichiShikai::SwordTwistEnd, "SwordTwist", 11);
	}
}

//---------------------------------------------// Sword Throw
void AOgichi::sh_SwordThrow()
{
	NewState		((uint8)EOgichiShikai::AttackB, "SwordThrow");
	SetRotation		(isMovingRight());
	AddImpulse		(MoveVector * 300, getFrameTime(2));
	SpawnHelper		("sh_SwordThrow", getFrameTime(5));

	SetBlockingAttack(EBlockType::Forward, getFrameTime(5), BLOCK_DURATION);
}

//---------------------------------------------// Getsuga Tensho
void AOgichi::sh_Getsuga()
{
	if (getHeroStatsComp()->checkStamina(-(GETSUGA_TENSHOU_COST))
		&& getHeroStatsComp()->checkPower(-(GETSUGA_TENSHOU_COST)))
	{
		NewState		((uint8)EOgichiShikai::Getsuga, "Getsuga");
		SetRotation		(isMovingRight());
		SpawnHelper		("sh_GetsugaHelper", getFrameTime(4));

		GET_STATS->AddStamina(GETSUGA_TENSHOU_COST, getFrameTime(2), true);
		SetBlockingAttack(EBlockType::Forward, getFrameTime(4), BLOCK_DURATION);
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------// Combo Implementation
void AOgichi::ComboI()
{
	Super::ComboI();

	FName form = getHeroStatsComp()->FormName;

	if (form == SHIKAI_NAME) { ShikaiComboI(); }
	else if (form == BANKAI_NAME) { BankaiComboI(); }
	else
	{

	}
}

void AOgichi::ShikaiComboI()
{
	EComboKey key = getNextKey();

	switch (getState())
	{

	case (uint8)EOgichiShikai::Attack_1: { if (key == EComboKey::CK_Attack) { sh_Attack_2(); }	break; }
	case (uint8)EOgichiShikai::Attack_2:
	{
		switch (key)
		{
		case EComboKey::CK_AForward:	{ sh_AttackFW();	break; }
		case EComboKey::CK_ABackward:	{ sh_SwordTwist();	break; }
		}
		break;
	}
	case (uint8)EOgichiShikai::SwordTwistLoop:
	{
		switch (key)
		{
		case EComboKey::CK_AForward:	{ sh_SwordThrow();	break; }
		case EComboKey::CK_ABackward:	{ sh_AttackB();		break; }
		}
		break;
	}


	} // End Switch
}

void AOgichi::BankaiComboI()
{

}