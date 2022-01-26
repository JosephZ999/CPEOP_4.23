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
	AddAnimation("Bankai",			ANIM_LOC_2 "Bankai");

	AnimData = &BankaiAnim;

	AddAnimation("Stand",			ANIM_LOC_2 "Stand");
	AddAnimation("Walk",			ANIM_LOC_2 "Run");
	AddAnimation("JumpStart",		ANIM_LOC_2 "JumpStart");
	AddAnimation("JumpUp",			ANIM_LOC_2 "JumpUp");
	AddAnimation("JumpHold",		ANIM_LOC_2 "JumpHold");
	AddAnimation("JumpDown",		ANIM_LOC_2 "JumpDown");
	AddAnimation("JumpLand",		ANIM_LOC_2 "JumpLand");

	AddAnimation("Hit",				ANIM_LOC_2 "Hit");
	AddAnimation("FallHold",		ANIM_LOC_2 "FallHold");
	AddAnimation("FallUp",			ANIM_LOC_2 "FallUp");
	AddAnimation("FallDown",		ANIM_LOC_2 "FallDown");
	AddAnimation("StandUp",			ANIM_LOC_2 "StandUp");
	AddAnimation("StandUpAir",		ANIM_LOC_2 "StandUpAir");

	AddAnimation("Block",			ANIM_LOC_2 "Block");
	AddAnimation("BlockAir",		ANIM_LOC_2 "Block");

	AddAnimation("PowChargeStart",	ANIM_LOC_2 "ChargeStart");
	AddAnimation("PowChargeLoop",	ANIM_LOC_2 "ChargeLoop");
	AddAnimation("PowChargeEnd",	ANIM_LOC_2 "ChargeEnd");

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
		case EOgichiState::SwordTwistLoop:
		{ 
			SET_TIMER(sh_STwistEndTimer, this, &AOgichi::sh_SwordTwistEnd, cTime(1.f));
			GET_STATS->AddStamina(0.07);
			break; 
		}
		case EOgichiState::Attack_1: { if (isComboTime()) { sh_Attack_2(); resetKeys(); } break; }
		
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
			if (IsSkillActive()) { sh_SwordTwist(); SkillDisable(); }
			else { sh_AttackB(); }
			break;
		}
		case EBaseStates::Jumping: { break; }

		case EOgichiState::SwordTwistLoop:
		{
			if (IsSkillActive()) { SkillDisable(); }
			else { sh_AttackB2(); }
			break;
		}
		case EOgichiState::Attack_2: { if (isComboTime()) { sh_SwordTwist(); resetKeys(); } break; }
		case EBaseStates::PowChargeLoop: { sh_SwordTwist(); SkillDisable(); break; }

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
		case EOgichiState::SwordTwistLoop:
		{ 
			if (IsSkillActive())	{ sh_Getsuga(); SkillDisable(); }
			else					{ sh_SwordThrow(); }
			break;
		}
		case EOgichiState::Attack_2: { if (isComboTime()) { sh_AttackFW(); resetKeys(); } break; }
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

//---------------------------------------------// Shikai Actions //////////////////////////////////
//---------------------------------------------// Attacks
void AOgichi::sh_Attack_1()
{
	FState nState;
	nState.State = EOgichiState::Attack_1;
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
	nState.State = EOgichiState::Attack_2;
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
	nState.State = EOgichiState::AttackFW;
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
	nState.State = EOgichiState::AttackB;
	nState.Animation = "AttackB";
	NewState(nState);

	AddImpulse		(SP_VELOCITY, getFrameTime(4));
	SpawnHelper		("sh_AttackBack", getFrameTime(6), FRotator(20.f, 0.f, 0.f));

	SetBlockingAttack(EBlockType::Forward, getFrameTime(6), BLOCK_DURATION);
}
void AOgichi::sh_AttackB2()
{
	FState nState;
	nState.State = EOgichiState::AttackB;
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
	nState.State = EOgichiState::SwordTwist;
	nState.Animation = "SwordTwist";
	nState.EndState = false;
	NewState(nState);

	SpawnHelper		("sh_SwordTwist", getFrameTime(4));
	SetBlockingAttack(EBlockType::Both, AnimElemTime(4), AnimElemTime(10));

	FTimerHandle timer;
	GetWorldTimerManager().SetTimer(timer, this, &AOgichi::sh_SwordTwistLoop, getFrameTime(10));
	
}
void AOgichi::sh_SwordTwistLoop()
{
	if (CheckState(EOgichiState::SwordTwist))
	{
		FState nState;
		nState.State = EOgichiState::SwordTwistLoop;
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
	if (CheckState(EOgichiState::SwordTwistLoop))
	{
		FState nState;
		nState.State = EOgichiState::SwordTwistEnd;
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
	nState.State = EOgichiState::SwordThrow;
	nState.Animation = "SwordThrow";
	NewState(nState);

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
		FState nState;
		nState.State = EOgichiState::Getsuga;
		nState.Animation = "Getsuga";
		NewState(nState);

		SpawnHelper		("sh_GetsugaHelper", getFrameTime(4));

		GET_STATS->AddStamina(GETSUGA_TENSHOU_COST, getFrameTime(2), true);
		SetBlockingAttack(EBlockType::Forward, getFrameTime(4), BLOCK_DURATION);
	}
}

// Bankai
void AOgichi::sh_Bankai()
{
	if (!getHeroStatsComp()->CheckSkill("Bankai"))
		return;

	FState nState;
	nState.State = EOgichiState::Bankai;
	nState.Animation = "Bankai";
	NewState(nState);

	SetImmortality(AnimElemTime(35));
	Bankai();
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

	case EOgichiState::Attack_1: { if (key == EComboKey::CK_Attack) { sh_Attack_2(); }	break; }
	case EOgichiState::Attack_2:
	{
		switch (key)
		{
		case EComboKey::CK_AForward:	{ sh_AttackFW();	break; }
		case EComboKey::CK_ABackward:	{ sh_SwordTwist();	break; }
		}
		break;
	}
	case EOgichiState::SwordTwistLoop:
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