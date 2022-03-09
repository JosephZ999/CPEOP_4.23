// Fill out your copyright notice in the Description page of Project Settings.

#include "Chars/Heroes/Ogichi.h"
#include "TimerManager.h"
#include "PaperFlipbook.h"

#define OGI_ANIM_LOC   "Texture/Chars/Ogichi/FBook/"
#define OGI_ANIM_LOC_2 "Texture/Chars/Ogichi_Bankai/Anim/"
#define OGI_HIT_LOC	   "Blueprint/Chars/Ogichi/Shikai/"

// Attack Options
#define BASE_VELOCITY  (MoveVector + GetActorForwardVector()) * 150
#define SP_VELOCITY	   (MoveVector + GetActorForwardVector()) * 200

#define BLOCK_DURATION cTime(0.2f)

AOgichi::AOgichi()
{
	if (GetHeroStats())
	{
		InitForm(SHIKAI_NAME, {4, 4, 7}); // 15
		InitForm(BANKAI_NAME, {6, 11, 9});
	}

	// Hit Boxes
	InitHelper("sh_Attack_1", OGI_HIT_LOC "sh_Attack_1");
	InitHelper("sh_Attack_2", OGI_HIT_LOC "sh_Attack_2");
	InitHelper("sh_AttackBack", OGI_HIT_LOC "sh_AttackBack");
	InitHelper("sh_AttackForward", OGI_HIT_LOC "sh_AttackForward");
	InitHelper("sh_SwordTwist", OGI_HIT_LOC "sh_SwordTwist");
	InitHelper("sh_SwordThrow", OGI_HIT_LOC "sh_SwordThrow");

	InitHelper("sh_GetsugaHelper", OGI_HIT_LOC "sh_GetsugaHelper");
	InitHelper("sh_GetsugaFWHelper", OGI_HIT_LOC "sh_GetsugaFWHelper");

	// Animations //
	// Ichi_Shikai
	AnimData = &ShikaiAnim;
	InitAnim("Stand", OGI_ANIM_LOC "Stand");
	InitAnim("Walk", OGI_ANIM_LOC "Walk");
	InitAnim("JumpStart", OGI_ANIM_LOC "JumpStart");
	InitAnim("JumpUp", OGI_ANIM_LOC "JumpUp");
	InitAnim("JumpHold", OGI_ANIM_LOC "JumpHold");
	InitAnim("JumpDown", OGI_ANIM_LOC "JumpDown");
	InitAnim("JumpLand", OGI_ANIM_LOC "JumpLand");

	InitAnim("Hit", OGI_ANIM_LOC "Hit1");
	InitAnim("FallHold", OGI_ANIM_LOC "FallHold");
	InitAnim("FallUp", OGI_ANIM_LOC "FallUp");
	InitAnim("FallDown", OGI_ANIM_LOC "FallDown");
	InitAnim("StandUp", OGI_ANIM_LOC "StandUp");
	InitAnim("StandUpAir", OGI_ANIM_LOC "StandUpAir");

	InitAnim("Block", OGI_ANIM_LOC "Block");
	InitAnim("BlockAir", OGI_ANIM_LOC "BlockAir");

	InitAnim("PowChargeStart", OGI_ANIM_LOC "PowChStart");
	InitAnim("PowChargeLoop", OGI_ANIM_LOC "PowChLoop");
	InitAnim("PowChargeEnd", OGI_ANIM_LOC "PowChEnd");

	InitAnim("Attack_1", OGI_ANIM_LOC "Attack1");
	InitAnim("Attack_2", OGI_ANIM_LOC "Attack2");
	InitAnim("AttackFW", OGI_ANIM_LOC "AttackForward");
	InitAnim("AttackB", OGI_ANIM_LOC "AttackBack");
	InitAnim("SwordTwist", OGI_ANIM_LOC "SwordTwist");
	InitAnim("SwordTwistLoop", OGI_ANIM_LOC "SwordTwistLoop");
	InitAnim("SwordThrow", OGI_ANIM_LOC "SwordThrow");

	InitAnim("Getsuga", OGI_ANIM_LOC "Getsuga");
	InitAnim("Bankai", OGI_ANIM_LOC_2 "Bankai");

	AnimData = &BankaiAnim;

	InitAnim("Stand", OGI_ANIM_LOC_2 "Stand");
	InitAnim("Walk", OGI_ANIM_LOC_2 "Run");
	InitAnim("JumpStart", OGI_ANIM_LOC_2 "JumpStart");
	InitAnim("JumpUp", OGI_ANIM_LOC_2 "JumpUp");
	InitAnim("JumpHold", OGI_ANIM_LOC_2 "JumpHold");
	InitAnim("JumpDown", OGI_ANIM_LOC_2 "JumpDown");
	InitAnim("JumpLand", OGI_ANIM_LOC_2 "JumpLand");

	InitAnim("Hit", OGI_ANIM_LOC_2 "Hit");
	InitAnim("FallHold", OGI_ANIM_LOC_2 "FallHold");
	InitAnim("FallUp", OGI_ANIM_LOC_2 "FallUp");
	InitAnim("FallDown", OGI_ANIM_LOC_2 "FallDown");
	InitAnim("StandUp", OGI_ANIM_LOC_2 "StandUp");
	InitAnim("StandUpAir", OGI_ANIM_LOC_2 "StandUpAir");

	InitAnim("Block", OGI_ANIM_LOC_2 "Block");
	InitAnim("BlockAir", OGI_ANIM_LOC_2 "Block");

	InitAnim("PowChargeStart", OGI_ANIM_LOC_2 "ChargeStart");
	InitAnim("PowChargeLoop", OGI_ANIM_LOC_2 "ChargeLoop");
	InitAnim("PowChargeEnd", OGI_ANIM_LOC_2 "ChargeEnd");

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

	if (GetHeroStats()->FormName == SHIKAI_NAME)
	{
		switch (GetState())
		{
		case EBaseStates::Stand:
		{
			sh_Attack_1();
			break;
		}
		case EBaseStates::Jumping:
		{
			break;
		}
		case EOgichiState::Ogi_SwordTwistLoop:
		{
			SET_TIMER(sh_STwistEndTimer, this, &AOgichi::sh_SwordTwistEnd, cTime(1.f));
			GET_STATS->AddStamina(0.07);
			break;
		}
		case EOgichiState::Ogi_Attack_1:
		{
			if (isComboTime()) { sh_Attack_2(); }
			break;
		}

		} // End Switch
	}
}
void AOgichi::AttackBack()
{
	Super::AttackBack();

	if (GetHeroStats()->FormName == SHIKAI_NAME)
	{
		switch (GetState())
		{
		case EBaseStates::Stand:
		{
			if (IsSkillActive()) { sh_SwordTwist(); }
			else
			{
				sh_AttackB();
			}
			break;
		}
		case EBaseStates::Jumping:
		{
			break;
		}

		case EOgichiState::Ogi_SwordTwistLoop:
		{
			if (IsSkillActive()) {}
			else
			{
				sh_AttackB();
			}
			break;
		}
		case EOgichiState::Ogi_Attack_2:
		{
			if (isComboTime()) { sh_SwordTwist(); }
			break;
		}
		case EBaseStates::PowChargeLoop:
		{
			sh_SwordTwist();
			break;
		}

		} // End Switch
	}
}
void AOgichi::AttackForward()
{
	Super::AttackForward();

	if (GetHeroStats()->FormName == SHIKAI_NAME)
	{
		switch (GetState())
		{
		case EBaseStates::Stand:
		{
			sh_AttackFW();
			break;
		}
		case EBaseStates::Jumping:
		{
			break;
		}
		case EOgichiState::Ogi_SwordTwistLoop:
		{
			if (IsSkillActive()) { sh_Getsuga(); }
			else
			{
				sh_SwordThrow();
			}
			break;
		}
		case EOgichiState::Ogi_Attack_2:
		{
			if (isComboTime()) { sh_AttackFW(); }
			break;
		}
		case EBaseStates::PowChargeLoop:
		{
			sh_Getsuga();
			break;
		}
		} // End Switch
	}
}

void AOgichi::AttackDown()
{
	if (GetHeroStats()->FormName == SHIKAI_NAME)
	{
		switch (GetState())
		{
		case EBaseStates::Stand:
		{
			if (IsSkillActive()) { sh_Bankai(); }
			break;
		}
		case EBaseStates::PowChargeLoop:
		{
			sh_Bankai();
			break;
		}
		}
	}
}

//---------------------------------------------// Ichi_Shikai Actions //////////////////////////////////
//---------------------------------------------// Attacks
void AOgichi::sh_Attack_1()
{
	FState nState;
	nState.State	 = EOgichiState::Ogi_Attack_1;
	nState.Animation = "Attack_1";
	NewState(nState);

	AddImpulse(BASE_VELOCITY, getFrameTime(2));
	SpawnHelperDeferred("sh_Attack_1", getFrameTime(4));
	Combo(getFrameTime(9));

	SetBlockingAttack(EBlockType::Forward, getFrameTime(4), BLOCK_DURATION);
}
void AOgichi::sh_Attack_2()
{
	FState nState;
	nState.State	 = EOgichiState::Ogi_Attack_2;
	nState.Animation = "Attack_2";
	NewState(nState);

	FVector impulse = BASE_VELOCITY;
	AddImpulse(impulse, getFrameTime(1));
	SpawnHelperDeferred("sh_Attack_2", getFrameTime(4));
	Combo(getFrameTime(9));

	SetBlockingAttack(EBlockType::Forward, getFrameTime(4), BLOCK_DURATION);
}
void AOgichi::sh_AttackFW()
{
	FState nState;
	nState.State	 = EOgichiState::Ogi_AttackFW;
	nState.Animation = "AttackFW";
	NewState(nState);

	AddImpulse(SP_VELOCITY, getFrameTime(1));
	SpawnHelperDeferred("sh_AttackForward", getFrameTime(5), FRotator(20.f, 0.f, 0.f));

	SetBlockingAttack(EBlockType::Forward, getFrameTime(5), BLOCK_DURATION);

	if (IsSkillActive() && GetHeroStats()->checkStamina(-(GETSUGA_COST)) && GetHeroStats()->checkPower(-(GETSUGA_COST)))
	{
		GET_STATS->AddStamina(GETSUGA_COST, getFrameTime(5), true);
		SpawnHelperDeferred("sh_GetsugaFWHelper", getFrameTime(5));
		SkillDisable();
	}
}
void AOgichi::sh_AttackB()
{
	FState nState;
	nState.State	 = EOgichiState::Ogi_AttackB;
	nState.Animation = "AttackB";
	NewState(nState);

	AddImpulse(SP_VELOCITY, getFrameTime(4));
	SpawnHelperDeferred("sh_AttackBack", getFrameTime(6), FRotator(20.f, 0.f, 0.f));

	SetBlockingAttack(EBlockType::Forward, getFrameTime(6), BLOCK_DURATION);
}

//---------------------------------------------// Sword Twist
void AOgichi::sh_SwordTwist()
{
	FState nState;
	nState.State	 = EOgichiState::Ogi_SwordTwist;
	nState.Animation = "SwordTwist";
	nState.EndState	 = false;
	NewState(nState);

	SpawnHelperDeferred("sh_SwordTwist", getFrameTime(4));
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
		nState.State	 = EOgichiState::Ogi_SwordTwistLoop;
		nState.Animation = "SwordTwistLoop";
		nState.EndState	 = false;
		nState.Rotate	 = false;
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
		nState.State		  = EOgichiState::Ogi_SwordTwistEnd;
		nState.Animation	  = "SwordTwist";
		nState.AnimationFrame = 11;
		nState.Rotate		  = false;
		NewState(nState);
	}
}

//---------------------------------------------// Sword Throw
void AOgichi::sh_SwordThrow()
{
	FState nState;
	nState.State	 = EOgichiState::Ogi_SwordThrow;
	nState.Animation = "SwordThrow";
	NewState(nState);

	AddImpulse(MoveVector * 300, getFrameTime(2));
	SpawnHelperDeferred("sh_SwordThrow", getFrameTime(5));

	SetBlockingAttack(EBlockType::Forward, getFrameTime(5), BLOCK_DURATION);
}

//---------------------------------------------// Ogi_Getsuga Tensho
void AOgichi::sh_Getsuga()
{
	if (GetHeroStats()->checkStamina(-(GETSUGA_TENSHOU_COST)) && GetHeroStats()->checkPower(-(GETSUGA_TENSHOU_COST)))
	{
		FState nState;
		nState.State	 = EOgichiState::Ogi_Getsuga;
		nState.Animation = "Getsuga";
		NewState(nState);

		SpawnHelperDeferred("sh_GetsugaHelper", getFrameTime(4));

		GET_STATS->AddStamina(GETSUGA_TENSHOU_COST, getFrameTime(2), true);
		SetBlockingAttack(EBlockType::Forward, getFrameTime(4), BLOCK_DURATION);
		SkillDisable();
	}
}

// Ogi_Bankai
void AOgichi::sh_Bankai()
{
	if (! GetHeroStats()->CheckSkill("Bankai")) return;

	FState nState;
	nState.State	 = EOgichiState::Ogi_Bankai;
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

	FName form = GetHeroStats()->FormName;

	if (form == SHIKAI_NAME) { ShikaiComboI(); }
	else if (form == BANKAI_NAME)
	{
		BankaiComboI();
	}
}

void AOgichi::ShikaiComboI()
{
	EComboKey key = getNextKey();

	switch (GetState())
	{

	case EOgichiState::Ogi_Attack_1:
	{
		if (key == EComboKey::CK_Attack) { sh_Attack_2(); }
		break;
	}
	case EOgichiState::Ogi_Attack_2:
	{
		switch (key)
		{
		case EComboKey::CK_AForward:
		{
			sh_AttackFW();
			break;
		}
		case EComboKey::CK_ABackward:
		{
			sh_SwordTwist();
			break;
		}
		}
		break;
	}
	case EOgichiState::Ogi_SwordTwistLoop:
	{
		switch (key)
		{
		case EComboKey::CK_AForward:
		{
			sh_SwordThrow();
			break;
		}
		case EComboKey::CK_ABackward:
		{
			sh_AttackB();
			break;
		}
		}
		break;
	}

	} // End Switch
}

void AOgichi::BankaiComboI() {}
