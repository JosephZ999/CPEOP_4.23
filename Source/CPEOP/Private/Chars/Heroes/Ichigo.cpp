// Fill out your copyright notice in the Description page of Project Settings.

#include "Chars/Heroes/Ichigo.h"
#include "Chars/AI/IchigoAI.h"
#include "UObject/ConstructorHelpers.h"

#define ICHI_ANIM_LOC	"Texture/Chars/Ichigo/FBook/"
#define ICHI_ANIM_LOC_B "Texture/Chars/Ichigo_Bankai/FBook/"
#define ICHI_HIT_LOC	"Blueprint/Chars/Ichigo/Shikai/"
#define ICHI_HIT_LOC_B	"Blueprint/Chars/Ichigo/Bankai/"

// Skills list
/*
	Getsuga
	SwordTwist
	SwordThrow
	RExplosion

	Bankai
	FastBankai
*/

AIchigo::AIchigo()
{
	SetHeroName("Ichigo");
	AIControllerClass = AIchigoAI::StaticClass();

	AfterImage = CreateDefaultSubobject<UAfterImageComponent>(TEXT("AfterImageComp"));

	if (GetHeroStats())
	{
		InitForm(SHIKAI_NAME, {4, 3, 3}); // 10
		InitForm(BANKAI_NAME, {6, 10, 5});
	}

	// Helpers
	{
		InitHelper("sh_Attack_1", ICHI_HIT_LOC "sh_Attack_1");
		InitHelper("sh_Attack_2", ICHI_HIT_LOC "sh_Attack_2");
		InitHelper("sh_AttackBack", ICHI_HIT_LOC "sh_AttackBack");
		InitHelper("sh_AttackForward", ICHI_HIT_LOC "sh_AttackForward");
		InitHelper("sh_AttackAir", ICHI_HIT_LOC "sh_AttackAir");

		InitHelper("sh_GetsugaHelper", ICHI_HIT_LOC "sh_GetsugaHelper");
		InitHelper("sh_GetsugaFWHelper", ICHI_HIT_LOC "sh_GetsugaFWHelper");
		InitHelper("sh_GetsugaExplosion", ICHI_HIT_LOC "sh_GetsugaExplosion");
		InitHelper("sh_ReiatsuExplosion", ICHI_HIT_LOC "sh_ReiatsuExplosion");
		InitHelper("sh_SwordTwist", ICHI_HIT_LOC "sh_SwordTwist");
		InitHelper("sh_SwordThrow", ICHI_HIT_LOC "sh_SwordThrow");
		InitHelper("BankaiEff", ICHI_HIT_LOC "BankaiEff");

		InitHelper("b_Attack_1", ICHI_HIT_LOC_B "b_Attack_1");
		InitHelper("b_Attack_2", ICHI_HIT_LOC_B "b_Attack_2");
		InitHelper("b_Attack_3", ICHI_HIT_LOC_B "b_Attack_3");
		InitHelper("b_Attack_4", ICHI_HIT_LOC_B "b_Attack_4");
		InitHelper("b_Attack_FW", ICHI_HIT_LOC_B "b_Attack_FW");
		InitHelper("b_Attack_FW_Slash", ICHI_HIT_LOC_B "b_Attack_FW_Slash");

		InitHelper("b_Attack_B", ICHI_HIT_LOC_B "b_Attack_B");
		InitHelper("b_AttackAir", ICHI_HIT_LOC_B "b_AttackAir");

		InitHelper("b_GetsugaEff", ICHI_HIT_LOC_B "GetsugaEff");
		InitHelper("b_Getsuga", ICHI_HIT_LOC_B "GetsugaHelper");
		InitHelper("b_RExplosion", ICHI_HIT_LOC_B "b_RExplosion");
	}

	// Shikai Animations
	{
		AnimData = &ShikaiAnim;
		InitAnim("Stand", ICHI_ANIM_LOC "Stand");
		InitAnim("Walk", ICHI_ANIM_LOC "Run");
		InitAnim("JumpStart", ICHI_ANIM_LOC "JumpStart");
		InitAnim("JumpUp", ICHI_ANIM_LOC "JumpUp");
		InitAnim("JumpHold", ICHI_ANIM_LOC "JumpHold");
		InitAnim("JumpDown", ICHI_ANIM_LOC "JumpDown");
		InitAnim("JumpLand", ICHI_ANIM_LOC "JumpLand");
		InitAnim("Hit", ICHI_ANIM_LOC "hit");
		InitAnim("FallHold", ICHI_ANIM_LOC "FallHold");
		InitAnim("FallUp", ICHI_ANIM_LOC "FallUp");
		InitAnim("FallDown", ICHI_ANIM_LOC "FallDown");
		InitAnim("StandUp", ICHI_ANIM_LOC "StandUp");
		InitAnim("StandUpAir", ICHI_ANIM_LOC "StandUpAir");
		InitAnim("Block", ICHI_ANIM_LOC "Guard");
		InitAnim("BlockAir", ICHI_ANIM_LOC "GuardAir");
		InitAnim("PowChargeStart", ICHI_ANIM_LOC "PowChStart");
		InitAnim("PowChargeLoop", ICHI_ANIM_LOC "PowChLoop");
		InitAnim("PowChargeEnd", ICHI_ANIM_LOC "PowChEnd");

		InitAnim("Attack_1", ICHI_ANIM_LOC "Attack1");
		InitAnim("Attack_2", ICHI_ANIM_LOC "Attack2");
		InitAnim("AttackFW", ICHI_ANIM_LOC "AttackForward");
		InitAnim("AttackB", ICHI_ANIM_LOC "AttackBack");
		InitAnim("AttackAir", ICHI_ANIM_LOC "AttackAir");

		InitAnim("SwordTwist", ICHI_ANIM_LOC "SwordTwist");
		InitAnim("SwordTwistLoop", ICHI_ANIM_LOC "SwordTwistLoop");
		InitAnim("SwordThrow", ICHI_ANIM_LOC "SwordThrow");
		InitAnim("GetsugaStart", ICHI_ANIM_LOC "GetsugaStart");
		InitAnim("GetsugaFW", ICHI_ANIM_LOC "GetsugaFW");
		InitAnim("RExplosion", ICHI_ANIM_LOC "RExplosion");

		InitAnim("Bankai", ICHI_ANIM_LOC "Bankai");
		InitAnim("FastBankai", ICHI_ANIM_LOC "BankaiFast");
	}

	// Bankai Animations
	{
		AnimData = &BankaiAnim;
		InitAnim("Stand", ICHI_ANIM_LOC_B "Stand");
		InitAnim("Walk", ICHI_ANIM_LOC_B "Run");
		InitAnim("JumpStart", ICHI_ANIM_LOC_B "JumpStart");
		InitAnim("JumpUp", ICHI_ANIM_LOC_B "JumpUp");
		InitAnim("JumpHold", ICHI_ANIM_LOC_B "JumpHold");
		InitAnim("JumpDown", ICHI_ANIM_LOC_B "JumpDown");
		InitAnim("JumpLand", ICHI_ANIM_LOC_B "JumpLand");
		InitAnim("Hit", ICHI_ANIM_LOC_B "Hit");
		InitAnim("FallHold", ICHI_ANIM_LOC_B "FallHold");
		InitAnim("FallUp", ICHI_ANIM_LOC_B "FallUp");
		InitAnim("FallDown", ICHI_ANIM_LOC_B "FallDown");
		InitAnim("StandUp", ICHI_ANIM_LOC_B "StandUp");
		InitAnim("StandUpAir", ICHI_ANIM_LOC_B "StandUpAir");
		InitAnim("Block", ICHI_ANIM_LOC_B "Guard");
		InitAnim("BlockAir", ICHI_ANIM_LOC_B "Guard");
		InitAnim("PowChargeStart", ICHI_ANIM_LOC_B "ChargeStart");
		InitAnim("PowChargeLoop", ICHI_ANIM_LOC_B "ChargeLoop");
		InitAnim("PowChargeEnd", ICHI_ANIM_LOC_B "ChargeEnd");

		InitAnim("Attack_1", ICHI_ANIM_LOC_B "Attack1");
		InitAnim("Attack_2", ICHI_ANIM_LOC_B "Attack2");
		InitAnim("Attack_3", ICHI_ANIM_LOC_B "Attack3");
		InitAnim("Attack_4", ICHI_ANIM_LOC_B "Attack4");
		InitAnim("Attack_FW", ICHI_ANIM_LOC_B "AttackFW");
		InitAnim("Attack_B", ICHI_ANIM_LOC_B "AttackB");
		InitAnim("AttackAir", ICHI_ANIM_LOC_B "AttackAir");

		InitAnim("GetsugaReady", ICHI_ANIM_LOC_B "GetsugaReady");
		InitAnim("Getsuga", ICHI_ANIM_LOC_B "Getsuga");
		InitAnim("GetsugaB", ICHI_ANIM_LOC_B "GetsugaB");
		InitAnim("RExplosion", ICHI_ANIM_LOC_B "RExplosion");

		InitAnim("Shikai", ICHI_ANIM_LOC_B "Shikai");
	}

	AnimData = &ShikaiAnim;

	b_AttackDashCurve = FindCurveFloat("Blueprint/Chars/Ichigo/Bankai/DashAttackVel");

	{
		ConstructorHelpers::FClassFinder<AAfterImage> nObject(
			(TEXT("Blueprint'/Game/Blueprint/Chars/Ichigo/Bankai/AfterImage_Stand.AfterImage_Stand_C'")));
		if (nObject.Succeeded())
		{
			AfterImage->ClassData.Add("Stand", nObject.Class);
		}
	}

	{
		ConstructorHelpers::FClassFinder<AAfterImage> nObject(
			(TEXT("Blueprint'/Game/Blueprint/Chars/Ichigo/Bankai/AfterImage_Run.AfterImage_Run_C'")));
		if (nObject.Succeeded())
		{
			AfterImage->ClassData.Add("Run", nObject.Class);
		}
	}
}

void AIchigo::Landed(const FHitResult& Hit)
{
	if (CheckState(EIchigoState::Ichi_Attack_Air))
	{
		EndState();
	}
	Super::Landed(Hit);
}

//---------------------------------------------// Inputs
void AIchigo::Attack()
{
	if (GET_STATS->FormName == SHIKAI_NAME)
	{
		sh_InputA();
	}
	else if (GET_STATS->FormName == BANKAI_NAME)
	{
		b_InputA();
	}
}

void AIchigo::AttackBack()
{
	if (GET_STATS->FormName == SHIKAI_NAME)
	{
		sh_InputB();
	}
	else if (GET_STATS->FormName == BANKAI_NAME)
	{
		b_InputB();
	}
}

void AIchigo::AttackForward()
{
	if (GET_STATS->FormName == SHIKAI_NAME)
	{
		sh_InputFW();
	}
	else if (GET_STATS->FormName == BANKAI_NAME)
	{
		b_InputFW();
	}
}

void AIchigo::AttackDown()
{
	if (GET_STATS->FormName == SHIKAI_NAME)
	{
		sh_InputD();
	}
	else if (GET_STATS->FormName == BANKAI_NAME)
	{
		b_InputD();
	}
}

//----------------------------------------------/ Combo
void AIchigo::ComboI()
{
	Super::ComboI();

	FName form = GetHeroStats()->FormName;

	if (form == SHIKAI_NAME)
	{
		ShikaiComboI();
	}
	else if (form == BANKAI_NAME)
	{
		BankaiComboI();
	}
}
