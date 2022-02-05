// Fill out your copyright notice in the Description page of Project Settings.


#include "Chars/Heroes/Ichigo.h"
#include "Chars/Components/ShadowComponent.h"

#define ICHI_ANIM_LOC		"Texture/Chars/Ichigo/FBook/"
#define ICHI_ANIM_LOC_B		"Texture/Chars/Ichigo_Bankai/FBook/"
#define ICHI_HIT_LOC		"Blueprint/Chars/Ichigo/Shikai/"
#define ICHI_HIT_LOC_B		"Blueprint/Chars/Ichigo/Bankai/"


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
	AfterImage = CreateDefaultSubobject<UAfterImageComponent>(TEXT("AfterImageComp"));

	if (getHeroStatsComp())
	{
		InitForm(SHIKAI_NAME, { 4, 3, 3 }); // 10
		InitForm(BANKAI_NAME, { 5, 8, 4 });
	}

	// Hit Boxes
	InitHelper("sh_Attack_1",			ICHI_HIT_LOC "sh_Attack_1");
	InitHelper("sh_Attack_2",			ICHI_HIT_LOC "sh_Attack_2");
	InitHelper("sh_AttackBack",			ICHI_HIT_LOC "sh_AttackBack");
	InitHelper("sh_AttackForward",		ICHI_HIT_LOC "sh_AttackForward");
	InitHelper("sh_AttackAir",			ICHI_HIT_LOC "sh_AttackAir");

	InitHelper("sh_GetsugaHelper",		ICHI_HIT_LOC "sh_GetsugaHelper");
	InitHelper("sh_GetsugaFWHelper",	ICHI_HIT_LOC "sh_GetsugaFWHelper");
	InitHelper("sh_GetsugaExplosion",	ICHI_HIT_LOC "sh_GetsugaExplosion");
	InitHelper("sh_ReiatsuExplosion",	ICHI_HIT_LOC "sh_ReiatsuExplosion");
	InitHelper("sh_SwordTwist",			ICHI_HIT_LOC "sh_SwordTwist");
	InitHelper("sh_SwordThrow",			ICHI_HIT_LOC "sh_SwordThrow");
	InitHelper("BankaiEff",				ICHI_HIT_LOC "BankaiEff");

	InitHelper("b_Attack_1",			ICHI_HIT_LOC_B "b_Attack_1");
	InitHelper("b_Attack_2",			ICHI_HIT_LOC_B "b_Attack_2");
	InitHelper("b_Attack_3",			ICHI_HIT_LOC_B "b_Attack_3");
	InitHelper("b_Attack_4",			ICHI_HIT_LOC_B "b_Attack_4");
	InitHelper("b_Attack_FW",			ICHI_HIT_LOC_B "b_Attack_FW");
	InitHelper("b_Attack_FW_Slash",		ICHI_HIT_LOC_B "b_Attack_FW_Slash");


	InitHelper("b_Attack_B",			ICHI_HIT_LOC_B "b_Attack_B");
	InitHelper("b_AttackAir",			ICHI_HIT_LOC_B "b_AttackAir");

	InitHelper("b_GetsugaEff",			ICHI_HIT_LOC_B "GetsugaEff");
	InitHelper("b_Getsuga",				ICHI_HIT_LOC_B "GetsugaHelper");
	InitHelper("b_RExplosion",			ICHI_HIT_LOC_B "b_RExplosion");

// Animations //
	// Ichi_Shikai
	AnimData = &ShikaiAnim;
	InitAnim("Stand",			ICHI_ANIM_LOC "Stand"		);
	InitAnim("Walk",			ICHI_ANIM_LOC "Run"			);
	InitAnim("JumpStart",		ICHI_ANIM_LOC "JumpStart"	);
	InitAnim("JumpUp",			ICHI_ANIM_LOC "JumpUp"		);
	InitAnim("JumpHold",		ICHI_ANIM_LOC "JumpHold"	);
	InitAnim("JumpDown",		ICHI_ANIM_LOC "JumpDown"	);
	InitAnim("JumpLand",		ICHI_ANIM_LOC "JumpLand"	);
	InitAnim("Hit",				ICHI_ANIM_LOC "hit"			);
	InitAnim("FallHold",		ICHI_ANIM_LOC "FallHold"	);
	InitAnim("FallUp",			ICHI_ANIM_LOC "FallUp"		);
	InitAnim("FallDown",		ICHI_ANIM_LOC "FallDown"	);
	InitAnim("StandUp",			ICHI_ANIM_LOC "StandUp"		);
	InitAnim("StandUpAir",		ICHI_ANIM_LOC "StandUpAir"	);
	InitAnim("Block",			ICHI_ANIM_LOC "Guard"		);
	InitAnim("BlockAir",		ICHI_ANIM_LOC "GuardAir"	);
	InitAnim("PowChargeStart",	ICHI_ANIM_LOC "PowChStart"	);
	InitAnim("PowChargeLoop",	ICHI_ANIM_LOC "PowChLoop"	);
	InitAnim("PowChargeEnd",	ICHI_ANIM_LOC "PowChEnd"	);

	InitAnim("Attack_1",		ICHI_ANIM_LOC "Attack1");
	InitAnim("Attack_2",		ICHI_ANIM_LOC "Attack2");
	InitAnim("AttackFW",		ICHI_ANIM_LOC "AttackForward");
	InitAnim("AttackB",			ICHI_ANIM_LOC "AttackBack");
	InitAnim("AttackAir",		ICHI_ANIM_LOC "AttackAir");

	InitAnim("SwordTwist",		ICHI_ANIM_LOC "SwordTwist");
	InitAnim("SwordTwistLoop",	ICHI_ANIM_LOC "SwordTwistLoop");
	InitAnim("SwordThrow",		ICHI_ANIM_LOC "SwordThrow");
	InitAnim("GetsugaStart",	ICHI_ANIM_LOC "GetsugaStart");
	InitAnim("GetsugaFW",		ICHI_ANIM_LOC "GetsugaFW");
	InitAnim("RExplosion",		ICHI_ANIM_LOC "RExplosion");

	InitAnim("Bankai",			ICHI_ANIM_LOC "Bankai");
	InitAnim("FastBankai",		ICHI_ANIM_LOC "BankaiFast");

	AnimData = &BankaiAnim;
	InitAnim("Stand",           ICHI_ANIM_LOC_B "Stand"	);
	InitAnim("Walk",            ICHI_ANIM_LOC_B "Run"		);
	InitAnim("JumpStart",       ICHI_ANIM_LOC_B "JumpStart");
	InitAnim("JumpUp",          ICHI_ANIM_LOC_B "JumpUp"	);
	InitAnim("JumpHold",        ICHI_ANIM_LOC_B "JumpHold"	);
	InitAnim("JumpDown",        ICHI_ANIM_LOC_B "JumpDown"	);
	InitAnim("JumpLand",        ICHI_ANIM_LOC_B "JumpLand"	);
	InitAnim("Hit",             ICHI_ANIM_LOC_B "Hit");
	InitAnim("FallHold",        ICHI_ANIM_LOC_B "FallHold");
	InitAnim("FallUp",          ICHI_ANIM_LOC_B "FallUp");
	InitAnim("FallDown",        ICHI_ANIM_LOC_B "FallDown");
	InitAnim("StandUp",         ICHI_ANIM_LOC_B "StandUp");
	InitAnim("StandUpAir",      ICHI_ANIM_LOC_B "StandUpAir");
	InitAnim("Block",           ICHI_ANIM_LOC_B "Guard");
	InitAnim("BlockAir",        ICHI_ANIM_LOC_B "Guard");
	InitAnim("PowChargeStart",  ICHI_ANIM_LOC_B "ChargeStart");
	InitAnim("PowChargeLoop",   ICHI_ANIM_LOC_B "ChargeLoop");
	InitAnim("PowChargeEnd",    ICHI_ANIM_LOC_B "ChargeEnd");

	InitAnim("Attack_1",        ICHI_ANIM_LOC_B "Attack1");
	InitAnim("Attack_2",        ICHI_ANIM_LOC_B "Attack2");
	InitAnim("Attack_3",        ICHI_ANIM_LOC_B "Attack3");
	InitAnim("Attack_4",        ICHI_ANIM_LOC_B "Attack4");
	InitAnim("Attack_FW",       ICHI_ANIM_LOC_B "AttackFW");
	InitAnim("Attack_B",        ICHI_ANIM_LOC_B "AttackB");
	InitAnim("AttackAir",		ICHI_ANIM_LOC_B "AttackAir");

	InitAnim("GetsugaReady",    ICHI_ANIM_LOC_B "GetsugaReady");
	InitAnim("Getsuga",         ICHI_ANIM_LOC_B "Getsuga");
	InitAnim("GetsugaB",        ICHI_ANIM_LOC_B "GetsugaB");
	InitAnim("RExplosion",      ICHI_ANIM_LOC_B "RExplosion");

	InitAnim("Shikai",          ICHI_ANIM_LOC_B "Shikai");

	AnimData = &ShikaiAnim;

	b_AttackDashCurve = FindCurveFloat("Blueprint/Chars/Ichigo/Bankai/DashAttackVel");
}

void AIchigo::BeginPlay()
{
	Super::BeginPlay();
	ChangeForm(SHIKAI_NAME);
}

void AIchigo::Landed(const FHitResult & Hit)
{
	if (CheckState(EIchigoState::Ichi_Attack_Air))
	{
		EndState();
	}

	Super::Landed(Hit);
}


// TimeLine Action
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
	if (!CheckState(EIchigoState::Ichi_Attack_FW))
	{
		StopTimeline();
	}
}

//---------------------------------------------// Inputs
	void AIchigo::Attack()
	{
		Super::Attack();
		
		if (getHeroStatsComp()->FormName == SHIKAI_NAME)
		{
			switch (GetState())
			{
			case EBaseStates::Stand: 
			{ 
				sh_Attack_1(); break; 
			}

			case EBaseStates::Jumping: 
			{ 
				sh_Attack_Air(); break;
			}

			case EIchigoState::Ichi_Attack_1:
			{ 
				if (isComboTime()) { sh_Attack_2(); } 
				break; 
			}
			case EIchigoState::Ichi_Attack_FW:
			{
				SkillEnable();
				break;
			}
			case EIchigoState::Ichi_Attack_B:
			{
				SkillEnable();
				break;
			}

			case EIchigoState::Ichi_SwordTwistLoop:
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
			switch (GetState())
			{
			case EBaseStates::Stand:   { b_Attack_1(); break; }
			case EBaseStates::Jumping: { b_Attack_Air(); break; }
			case EIchigoState::Ichi_Attack_1:
			{
				if (isComboTime()) { b_Attack_2(); }
				break;
			}
			case EIchigoState::Ichi_Attack_2:
			{
				if (isComboTime()) { b_Attack_3(); }
				break;
			}
			case EIchigoState::Ichi_Attack_3:
			{
				if (isComboTime()) { b_Attack_4(); }
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
			switch (GetState())
			{
			case EBaseStates::Stand:
			{
				if (IsSkillActive())
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
			case EIchigoState::Ichi_Attack_2: { if (isComboTime()) { sh_AttackB(); } break; }
			case EBaseStates::PowChargeLoop: { sh_SwordTwist(); break; }
			case EIchigoState::Ichi_SwordTwistLoop:
			{
				if (IsSkillActive())
				{
					sh_RExplosion(); SkillDisable();
				}
				else
				{
					sh_AttackB();
				}
				break;
			}

			} // End Switch
		}

		if (getHeroStatsComp()->FormName == BANKAI_NAME)
		{
			switch (GetState())
			{
			case EBaseStates::Stand: 
			{ 
				if (IsSkillActive())
				{
					b_RExplosion();
				}
				else
				{
					b_Attack_B(); 
				}
				
				break; 
			}
			case EIchigoState::Ichi_Attack_2:
			{
				if (isComboTime())
				{
					b_Attack_B();
				}
				break;
			}
			case EIchigoState::Ichi_Attack_3:
			{
				if (isComboTime())
				{
					b_Attack_B();
				}
				break;
			}
			case EBaseStates::PowChargeLoop: 
			{ 
				b_RExplosion();
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
					sh_AttackFW();
				}
				break;
			}
			case EBaseStates::Jumping: { break; }
			case EIchigoState::Ichi_Attack_2: { if (isComboTime()) { sh_AttackFW(); } break; }
			case EBaseStates::PowChargeLoop: { sh_GetsugaStart(); break; }
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

		if (getHeroStatsComp()->FormName == BANKAI_NAME)
		{
			switch (GetState())
			{
			case EBaseStates::Stand:
			{
				if (IsSkillActive())
				{
					b_Getsuga();
				}
				else
				{
					b_Attack_FW();
				}
				break;
			}
			case EBaseStates::Jumping: { break; }
			case EIchigoState::Ichi_Attack_2: { if (isComboTime()) { b_Attack_FW(); } break; }
			case EIchigoState::Ichi_Attack_3: { if (isComboTime()) { b_Attack_FW(); } break; }
			
			case EBaseStates::PowChargeLoop: 
			{ 
				b_Getsuga();
				break; 
			}
			
			} // End Switch
		}
	}

	void AIchigo::Btn_Bankai()
	{
		if (getHeroStatsComp()->FormName == SHIKAI_NAME)
		{
			switch (GetState())
			{
			case EBaseStates::Stand: { if (IsSkillActive()) { sh_Bankai(); } break; }
			case EBaseStates::PowChargeLoop: { sh_Bankai(); break; }
			}
		}

		if (getHeroStatsComp()->FormName == BANKAI_NAME)
		{
			switch (GetState())
			{
			case EBaseStates::Stand: { if (IsSkillActive()) { b_Shikai(); } break; }
			case EBaseStates::PowChargeLoop: { b_Shikai(); break; }
			}
		}
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

	//-----------------------------------//

	void AIchigo::ShikaiComboI()
	{
		EComboKey key = getNextKey();

		switch (GetState())
		{
		case EIchigoState::Ichi_Attack_1:
		{ 
			switch (key)
			{
			case EComboKey::CK_Attack:	{ sh_Attack_2(); break; }
			case EComboKey::CK_Dash:	{ DoDash();		 break; }
			}
			break;
		}
		case EIchigoState::Ichi_Attack_2:
		{
			switch (key)
			{
			case EComboKey::CK_AForward:	{ sh_AttackFW(); break; }
			case EComboKey::CK_ABackward:	{ sh_AttackB();	 break; }
			case EComboKey::CK_Dash:		{ DoDash();		 break; }
			}
			break;
		}

		case EIchigoState::Ichi_Attack_B:
		{
			if (key == EComboKey::CK_Dash)
			{
				DoDash();
			}
			break;
		}
		case EIchigoState::Ichi_Attack_FW:
		{
			if (key == EComboKey::CK_Dash)
			{
				DoDash();
			}
			break;
		}
		case EIchigoState::Ichi_SwordTwist:
		{
			switch (key)
			{
			case EComboKey::CK_AForward: { sh_SwordThrow();	break; }
			case EComboKey::CK_ABackward: { sh_AttackB();		break; }
			}
			break;
		}
		case EIchigoState::Ichi_SwordTwistLoop:
		{
			switch (key)
			{
			case EComboKey::CK_AForward:  { sh_SwordThrow();	break; }
			case EComboKey::CK_ABackward: { sh_AttackB();		break; }
			}
			break;
		}

		case EIchigoState::Ichi_SwordThrow:
		{
			if (key == EComboKey::CK_Dash)
			{
				DoDash();
			}
			break;
		}

		case EIchigoState::Ichi_GetsugaStart:
		{
			sh_GetsugaSlash();
			break;
		}
		} // End Switch
	}

	//-----------------------------------//

	void AIchigo::BankaiComboI()
	{
		EComboKey key = getNextKey();

		switch (GetState())
		{
		case EIchigoState::Ichi_Attack_1:
		{
			switch (key)
			{
			case EComboKey::CK_Attack: { b_Attack_2(); break; }
			case EComboKey::CK_Dash:   { DoDash();     break; }
			} // Switch End
			break;
		}
		case EIchigoState::Ichi_Attack_2:
		{
			switch (key)
			{
			case EComboKey::CK_Attack:    { b_Attack_3();  break; }
			case EComboKey::CK_ABackward: { b_Attack_B();  break; }
			case EComboKey::CK_AForward:  { b_Attack_FW(); break; }
			case EComboKey::CK_Dash:      { DoDash();      break; }
			} // Switch End
			break;
		}
		case EIchigoState::Ichi_Attack_3:
		{
			switch (key)
			{
			case EComboKey::CK_Dash:      { DoDash();      break; }
			} // Switch End
			break;
		}
		case EIchigoState::Ichi_Attack_4:
		{
			switch (key)
			{
			case EComboKey::CK_Dash:      { DoDash();      break; }
			} // Switch End
			break;
		}

		case EIchigoState::Ichi_Attack_B:
		{
			if (key == EComboKey::CK_Dash)
			{
				DoDash();
			}
			break;
		}
		case EIchigoState::Ichi_Attack_FW:
		{
			if (key == EComboKey::CK_Attack)
			{
				b_Attack_FW_Slash();
			}
			else if (key == EComboKey::CK_Dash)
			{
				DoDash();
			}
			break;
		}
		case EIchigoState::Ichi_Attack_FW_Slash:
		{
			if (key == EComboKey::CK_Attack)
			{
				b_Attack_FW_Slash();
			}
			else
			{
				b_Attack_FW_End();
			}
			break;
		}
		case EIchigoState::Ichi_GetsugaStart:
		{
			if (key == EComboKey::CK_ABackward)
			{
				b_GetsugaB();
			}
			else
			{
				b_GetsugaFW();
			}
			break;
		}
		case EIchigoState::Ichi_GetsugaFW:
		{
			if (key == EComboKey::CK_Dash)
			{
				DoDash();
			}
			break;
		}

		} // Switch End
	}