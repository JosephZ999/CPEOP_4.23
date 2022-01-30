// Fill out your copyright notice in the Description page of Project Settings.

#include "Chars/Heroes/Ogichi.h"
#include "TimerManager.h"
#include "PaperFlipbook.h"

#define ANIM_LOC	"Texture/Chars/Ogichi/FBook/"
#define ANIM_LOC_2	"Texture/Chars/Ogichi_Bankai/Anim/"
#define HIT_LOC		"Blueprint/Chars/Ogichi/Shikai/"

// Attack Options
#define BASE_VELOCITY	(MoveVector + GetActorForwardVector()) * 150
#define SP_VELOCITY		(MoveVector + GetActorForwardVector()) * 200

#define BLOCK_DURATION cTime(0.2f)



AOgichi::AOgichi()
{
	if (getHeroStatsComp())
	{
		InitForm(SHIKAI_NAME, { 4, 4, 7 }); // 15
		InitForm(BANKAI_NAME, { 5, 9, 8 });
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
	// Ichi_Shikai
	AnimData = &ShikaiAnim;
	InitAnim("Stand",			ANIM_LOC "Stand");
	InitAnim("Walk",			ANIM_LOC "Walk");
	InitAnim("JumpStart",		ANIM_LOC "JumpStart");
	InitAnim("JumpUp",			ANIM_LOC "JumpUp");
	InitAnim("JumpHold",		ANIM_LOC "JumpHold");
	InitAnim("JumpDown",		ANIM_LOC "JumpDown");
	InitAnim("JumpLand",		ANIM_LOC "JumpLand");

	InitAnim("Hit",				ANIM_LOC "Hit1");
	InitAnim("FallHold",		ANIM_LOC "FallHold");
	InitAnim("FallUp",			ANIM_LOC "FallUp");
	InitAnim("FallDown",		ANIM_LOC "FallDown");
	InitAnim("StandUp",			ANIM_LOC "StandUp");
	InitAnim("StandUpAir",		ANIM_LOC "StandUpAir");

	InitAnim("Block",			ANIM_LOC "Block");
	InitAnim("BlockAir",		ANIM_LOC "BlockAir");

	InitAnim("PowChargeStart",	ANIM_LOC "PowChStart");
	InitAnim("PowChargeLoop",	ANIM_LOC "PowChLoop");
	InitAnim("PowChargeEnd",	ANIM_LOC "PowChEnd");

	InitAnim("Attack_1",		ANIM_LOC "Attack1");
	InitAnim("Attack_2",		ANIM_LOC "Attack2");
	InitAnim("AttackFW",		ANIM_LOC "AttackForward");
	InitAnim("AttackB",			ANIM_LOC "AttackBack");
	InitAnim("AttackB2",		ANIM_LOC "AttackBack2");
	InitAnim("SwordTwist",		ANIM_LOC "SwordTwist");
	InitAnim("SwordTwistLoop",	ANIM_LOC "SwordTwistLoop");
	InitAnim("SwordThrow",		ANIM_LOC "SwordThrow");

	InitAnim("Getsuga",			ANIM_LOC "Getsuga");
	InitAnim("Bankai",			ANIM_LOC_2 "Bankai");

	AnimData = &BankaiAnim;

	InitAnim("Stand",			ANIM_LOC_2 "Stand");
	InitAnim("Walk",			ANIM_LOC_2 "Run");
	InitAnim("JumpStart",		ANIM_LOC_2 "JumpStart");
	InitAnim("JumpUp",			ANIM_LOC_2 "JumpUp");
	InitAnim("JumpHold",		ANIM_LOC_2 "JumpHold");
	InitAnim("JumpDown",		ANIM_LOC_2 "JumpDown");
	InitAnim("JumpLand",		ANIM_LOC_2 "JumpLand");

	InitAnim("Hit",				ANIM_LOC_2 "Hit");
	InitAnim("FallHold",		ANIM_LOC_2 "FallHold");
	InitAnim("FallUp",			ANIM_LOC_2 "FallUp");
	InitAnim("FallDown",		ANIM_LOC_2 "FallDown");
	InitAnim("StandUp",			ANIM_LOC_2 "StandUp");
	InitAnim("StandUpAir",		ANIM_LOC_2 "StandUpAir");

	InitAnim("Block",			ANIM_LOC_2 "Block");
	InitAnim("BlockAir",		ANIM_LOC_2 "Block");

	InitAnim("PowChargeStart",	ANIM_LOC_2 "ChargeStart");
	InitAnim("PowChargeLoop",	ANIM_LOC_2 "ChargeLoop");
	InitAnim("PowChargeEnd",	ANIM_LOC_2 "ChargeEnd");

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
		switch (GetState())
		{
		case EBaseStates::Stand:		{ sh_Attack_1(); break; }
		case EBaseStates::Jumping:		{ break; }
		case EOgichiState::Ogi_SwordTwistLoop:
		{ 
			SET_TIMER(sh_STwistEndTimer, this, &AOgichi::sh_SwordTwistEnd, cTime(1.f));
			GET_STATS->AddStamina(0.07);
			break; 
		}
		case EOgichiState::Ogi_Attack_1: { if (isComboTime()) { sh_Attack_2(); resetKeys(); } break; }
		
		} // End Switch

	}
}
void AOgichi::AttackBack()
{
	Super::AttackBack();

	if (getHeroStatsComp()->FormName == SHIKAI_NAME)
	{
		switch (GetState())
		{
		case EBaseStates::Stand:
		{
			if (IsSkillActive()) { sh_SwordTwist(); }
			else { sh_AttackB(); }
			break;
		}
		case EBaseStates::Jumping: { break; }

		case EOgichiState::Ogi_SwordTwistLoop:
		{
			if (IsSkillActive()) { }
			else { sh_AttackB2(); }
			break;
		}
		case EOgichiState::Ogi_Attack_2: { if (isComboTime()) { sh_SwordTwist(); resetKeys(); } break; }
		case EBaseStates::PowChargeLoop: { sh_SwordTwist(); break; }

		} // End Switch
	}
}
void AOgichi::AttackForward()
{
	Super::AttackForward();

	if (getHeroStatsComp()->FormName == SHIKAI_NAME)
	{
		switch (GetState())
		{
		case EBaseStates::Stand:	
		{ 
			sh_AttackFW();
			break;
		}
		case EBaseStates::Jumping:	{ break; }
		case EOgichiState::Ogi_SwordTwistLoop:
		{ 
			if (IsSkillActive())	{ sh_Getsuga(); }
			else					{ sh_SwordThrow(); }
			break;
		}
		case EOgichiState::Ogi_Attack_2: { if (isComboTime()) { sh_AttackFW(); resetKeys(); } break; }
		case EBaseStates::PowChargeLoop: { sh_Getsuga(); break; }
		} // End Switch
	}
}

void AOgichi::Btn_Bankai()
{
	if (getHeroStatsComp()->FormName == SHIKAI_NAME)
	{
		switch (GetState())
		{
		case EBaseStates::Stand: { if (IsSkillActive()) { sh_Bankai(); } break; }
		case EBaseStates::PowChargeLoop: { sh_Bankai(); break; }
		}
	}
}

//---------------------------------------------// Ichi_Shikai Actions //////////////////////////////////
//---------------------------------------------// Attacks
void AOgichi::sh_Attack_1()
{
	FState nState;
	nState.State = EOgichiState::Ogi_Attack_1;
	nState.Animation = "Attack_1";
	NewState(nState);

	AddImpulse		(BASE_VELOCITY, getFrameTime(2));
	SpawnHelper		("sh_Attack_1", getFrameTime(4));
	Combo			(getFrameTime(9));

	SetBlockingAttack(EBlockType::Forward, getFrameTime(4), BLOCK_DURATION);
}
void AOgichi::sh_Attack_2()
{
	FState nState;
	nState.State = EOgichiState::Ogi_Attack_2;
	nState.Animation = "Attack_2";
	NewState(nState);

	AddImpulse		(BASE_VELOCITY, getFrameTime(1));
	SpawnHelper		("sh_Attack_2", getFrameTime(4));
	Combo			(getFrameTime(9));

	SetBlockingAttack(EBlockType::Forward, getFrameTime(4), BLOCK_DURATION);
}
void AOgichi::sh_AttackFW()
{
	FState nState;
	nState.State = EOgichiState::Ogi_AttackFW;
	nState.Animation = "AttackFW";
	NewState(nState);

	AddImpulse		(SP_VELOCITY, getFrameTime(1));
	SpawnHelper		("sh_AttackForward", getFrameTime(5), FRotator(20.f, 0.f, 0.f));

	SetBlockingAttack(EBlockType::Forward, getFrameTime(5), BLOCK_DURATION);

	if (IsSkillActive()
		&& getHeroStatsComp()->checkStamina(-(GETSUGA_COST))
		&& getHeroStatsComp()->checkPower(-(GETSUGA_COST)))
	{
		GET_STATS->AddStamina(GETSUGA_COST, getFrameTime(5), true);
		SpawnHelper("sh_GetsugaFWHelper", getFrameTime(5));
		SkillDisable();
	}
}
void AOgichi::sh_AttackB()
{
	FState nState;
	nState.State = EOgichiState::Ogi_AttackB;
	nState.Animation = "AttackB";
	NewState(nState);

	AddImpulse		(SP_VELOCITY, getFrameTime(4));
	SpawnHelper		("sh_AttackBack", getFrameTime(6), FRotator(20.f, 0.f, 0.f));

	SetBlockingAttack(EBlockType::Forward, getFrameTime(6), BLOCK_DURATION);
}
void AOgichi::sh_AttackB2()
{
	FState nState;
	nState.State = EOgichiState::Ogi_AttackB;
	nState.Animation = "AttackB2";
	NewState(nState);

	AddImpulse		(SP_VELOCITY, getFrameTime(7));
	SpawnHelper		("sh_AttackBack", getFrameTime(9), FRotator(20.f, 0.f, 0.f));

	SetBlockingAttack(EBlockType::Forward, getFrameTime(9), BLOCK_DURATION);
}

//---------------------------------------------// Sword Twist
void AOgichi::sh_SwordTwist()
{
	FState nState;
	nState.State = EOgichiState::Ogi_SwordTwist;
	nState.Animation = "SwordTwist";
	nState.EndState = false;
	NewState(nState);

	SpawnHelper		("sh_SwordTwist", getFrameTime(4));
	SetBlockingAttack(EBlockType::Both, AnimElemTime(4), AnimElemTime(10));

	FTimerHandle timer;
	GetWorldTimerManager().SetTimer(timer, this, &AOgichi::sh_SwordTwistLoop, getFrameTime(10));
	SkillDisable();
}
void AOgichi::sh_SwordTwistLoop()
{
	if (CheckState(EOgichiState::Ogi_SwordTwist))
	{
		FState nState;
		nState.State = EOgichiState::Ogi_SwordTwistLoop;
		nState.Animation = "SwordTwistLoop";
		nState.EndState = false;
		nState.Rotate = false;
		NewState(nState);

		GetWorldTimerManager().SetTimer(sh_STwistEndTimer, this, &AOgichi::sh_SwordTwistEnd, cTime(1.f));
		Combo(getFrameTime(3));
	}
}
void AOgichi::sh_SwordTwistEnd()
{
	if (CheckState(EOgichiState::Ogi_SwordTwistLoop))
	{
		FState nState;
		nState.State = EOgichiState::Ogi_SwordTwistEnd;
		nState.Animation = "SwordTwist";
		nState.AnimationFrame = 11;
		nState.Rotate = false;
		NewState(nState);
	}
}

//---------------------------------------------// Sword Throw
void AOgichi::sh_SwordThrow()
{
	FState nState;
	nState.State = EOgichiState::Ogi_SwordThrow;
	nState.Animation = "SwordThrow";
	NewState(nState);

	AddImpulse		(MoveVector * 300, getFrameTime(2));
	SpawnHelper		("sh_SwordThrow", getFrameTime(5));

	SetBlockingAttack(EBlockType::Forward, getFrameTime(5), BLOCK_DURATION);
}

//---------------------------------------------// Ogi_Getsuga Tensho
void AOgichi::sh_Getsuga()
{
	if (getHeroStatsComp()->checkStamina(-(GETSUGA_TENSHOU_COST))
		&& getHeroStatsComp()->checkPower(-(GETSUGA_TENSHOU_COST)))
	{
		FState nState;
		nState.State = EOgichiState::Ogi_Getsuga;
		nState.Animation = "Getsuga";
		NewState(nState);

		SpawnHelper		("sh_GetsugaHelper", getFrameTime(4));

		GET_STATS->AddStamina(GETSUGA_TENSHOU_COST, getFrameTime(2), true);
		SetBlockingAttack(EBlockType::Forward, getFrameTime(4), BLOCK_DURATION);
		SkillDisable();
	}
}

// Ogi_Bankai
void AOgichi::sh_Bankai()
{
	if (!getHeroStatsComp()->CheckSkill("Bankai"))
		return;

	FState nState;
	nState.State = EOgichiState::Ogi_Bankai;
	nState.Animation = "Bankai";
	NewState(nState);

	SetImmortality(AnimElemTime(35));
	Bankai();
	SkillDisable();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------// Combo Implementation
void AOgichi::ComboI()
{
	Super::ComboI();

	FName form = getHeroStatsComp()->FormName;

	if (form == SHIKAI_NAME)		{ ShikaiComboI(); }
	else if (form == BANKAI_NAME)	{ BankaiComboI(); }
}

void AOgichi::ShikaiComboI()
{
	EComboKey key = getNextKey();

	switch (GetState())
	{

	case EOgichiState::Ogi_Attack_1: { if (key == EComboKey::CK_Attack) { sh_Attack_2(); }	break; }
	case EOgichiState::Ogi_Attack_2:
	{
		switch (key)
		{
		case EComboKey::CK_AForward:	{ sh_AttackFW();	break; }
		case EComboKey::CK_ABackward:	{ sh_SwordTwist();	break; }
		}
		break;
	}
	case EOgichiState::Ogi_SwordTwistLoop:
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