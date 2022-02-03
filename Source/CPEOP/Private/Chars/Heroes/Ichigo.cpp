// Fill out your copyright notice in the Description page of Project Settings.


#include "Chars/Heroes/Ichigo.h"
#include "Chars/Components/ShadowComponent.h"

#define ICHI_ANIM_LOC		"Texture/Chars/Ichigo/FBook/"
#define ICHI_ANIM_LOC_B		"Texture/Chars/Ichigo_Bankai/FBook/"
#define ICHI_HIT_LOC		"Blueprint/Chars/Ichigo/Shikai/"
#define ICHI_HIT_LOC_B		"Blueprint/Chars/Ichigo/Bankai/"

/* Attack Options */
#define BASE_VELOCITY	(MoveVector + GetActorForwardVector()) * 150
#define SP_VELOCITY		(MoveVector + GetActorForwardVector()) * 200

#define BLOCK_DURATION cTime(0.2f)

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

	InitAnim("Getsuga",         ICHI_ANIM_LOC_B "Getsuga");
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

//---------------------------------------------// Ichi_Shikai Actions //////////////////////////////////
//---------------------------------------------// Attacks
	void AIchigo::sh_Attack_1()
	{
		FState nState;
		nState.State = EIchigoState::Ichi_Attack_1;
		nState.Animation = "Attack_1";
		NewState( nState );

		AddImpulse		(BASE_VELOCITY, getFrameTime(2));
		SpawnHelper		("sh_Attack_1", getFrameTime(4));
		Combo			(getFrameTime(9));

		SetBlockingAttack(EBlockType::Forward, getFrameTime(4), BLOCK_DURATION);
		DangerN(getFrameTime(6), EDangerType::MeleeAttack);



	}
	void AIchigo::sh_Attack_2()
	{
		FState nState;
		nState.State = EIchigoState::Ichi_Attack_2;
		nState.Animation = "Attack_2";
		NewState(nState);

		AddImpulse		(BASE_VELOCITY, getFrameTime(1));
		SpawnHelper		("sh_Attack_2", getFrameTime(4));
		Combo			(getFrameTime(9));

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

	void AIchigo::sh_AttackFW()
	{
		FState nState;
		nState.State = EIchigoState::Ichi_Attack_FW;
		nState.Animation = "AttackFW";
		NewState(nState);

		AddImpulse		(SP_VELOCITY, getFrameTime(1));
		SpawnHelper		("sh_AttackForward", getFrameTime(5));
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
			if (!getHeroStatsComp()->checkStamina(-(GETSUGA_COST)))
			{
				NotEnoughStamina();
				return;
			}

			if (!getHeroStatsComp()->checkPower(-(GETSUGA_COST)))
			{
				NotEnoughPower();
				return;
			}

			GET_STATS->AddStamina(GETSUGA_COST, 0.f, true);
			SpawnHelper("sh_GetsugaFWHelper");
			SkillDisable();
		}
	}

	void AIchigo::sh_AttackB()
	{
		FState nState;
		nState.State = EIchigoState::Ichi_Attack_B;
		nState.Animation = "AttackB";
		NewState(nState);

		AddImpulse		(SP_VELOCITY, getFrameTime(4));
		SpawnHelper		("sh_AttackBack", getFrameTime(6));
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
			if (!getHeroStatsComp()->checkStamina(-(GETSUGA_COST)))
			{
				NotEnoughStamina();
				return;
			}

			if (!getHeroStatsComp()->checkPower(-(GETSUGA_COST)))
			{
				NotEnoughPower();
				return;
			}

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

	//---------------------------------------------// Getsuga Tensho
	void AIchigo::sh_GetsugaStart()
	{
		if (!getHeroStatsComp()->checkStamina(-(GETSUGA_TENSHOU_COST)))
		{
			NotEnoughStamina();
			return;
		}
		
		if (!getHeroStatsComp()->checkPower(-(GETSUGA_TENSHOU_COST)))
		{
			NotEnoughPower();
			return;
		}
		
		FState nState;
		nState.State = EIchigoState::Ichi_GetsugaStart;
		nState.Animation = "GetsugaStart";
		NewState(nState);

		Combo(getFrameTime(10));
		SkillDisable();
	}

	void AIchigo::sh_GetsugaSlash()
	{
		FState nState;
		nState.State = EIchigoState::Ichi_GetsugaFW;
		nState.Animation = "GetsugaFW";
		NewState(nState);

		SpawnHelper		("sh_GetsugaHelper", getFrameTime(3));
		GET_STATS->AddStamina(GETSUGA_TENSHOU_COST, getFrameTime(2), true, GetState());

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

		if (!getHeroStatsComp()->checkStamina(-(EXPLOSION_COST)))
		{
			NotEnoughStamina();
			return;
		}

		if (!getHeroStatsComp()->checkPower(-(EXPLOSION_COST)))
		{
			NotEnoughPower();
			return;
		}

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

	// Ichi_Bankai

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

//---------------------------------------------// Ichi_Bankai Actions //////////////////////////////////
	void AIchigo::b_Attack_1()
	{
		FState nState;
		nState.State = EIchigoState::Ichi_Attack_1;
		nState.Animation = "Attack_1";
		NewState(nState);

		AddImpulse(BASE_VELOCITY, getFrameTime(1));
		SpawnHelper("b_Attack_1", getFrameTime(3));
		Combo(getFrameTime(7));

		SetBlockingAttack(EBlockType::Forward, getFrameTime(3), BLOCK_DURATION);
		DangerN(getFrameTime(6), EDangerType::MeleeAttack);
	}

	void AIchigo::b_Attack_2()
	{
		FState nState;
		nState.State = EIchigoState::Ichi_Attack_2;
		nState.Animation = "Attack_2";
		NewState(nState);

		AddImpulse(BASE_VELOCITY, getFrameTime(1));
		SpawnHelper("b_Attack_2", getFrameTime(3));
		Combo(getFrameTime(7));

		SetBlockingAttack(EBlockType::Forward, getFrameTime(3), BLOCK_DURATION);
		DangerN(getFrameTime(6), EDangerType::MeleeAttack);
	}

	void AIchigo::b_Attack_3()
	{
		FState nState;
		nState.State = EIchigoState::Ichi_Attack_3;
		nState.Animation = "Attack_3";
		NewState(nState);

		AddImpulse(BASE_VELOCITY, getFrameTime(3));
		SpawnHelper("b_Attack_3", getFrameTime(4));
		Combo(getFrameTime(9));

		SetBlockingAttack(EBlockType::Both, getFrameTime(3), getFrameTime(4));
		DangerN(getFrameTime(6), EDangerType::MeleeAttack);
	}

	void AIchigo::b_Attack_4()
	{
		FState nState;
		nState.State = EIchigoState::Ichi_Attack_4;
		nState.Animation = "Attack_4";
		NewState(nState);

		AddImpulse(BASE_VELOCITY, getFrameTime(2));
		SpawnHelper("b_Attack_4", getFrameTime(3));
		Combo(getFrameTime(10));

		SetBlockingAttack(EBlockType::Forward, getFrameTime(3), BLOCK_DURATION);
		DangerN(getFrameTime(4), EDangerType::MeleeAttack);
	}

	void AIchigo::b_Attack_FW()
	{
		if (!GET_STATS->checkStamina(2.f / getHeroStatsComp()->getTeleportCost(), false))
		{
			NotEnoughStamina();
			return;
		}

		FState nState;
		nState.State = EIchigoState::Ichi_Attack_FW;
		nState.Animation = "Attack_FW";
		NewState(nState);

		// Helpers
		SpawnHelper("b_Attack_FW", getFrameTime(5));
		SpawnHelper("Teleport", getFrameTime(3), GetActorRotation());

		// Camera Behaviour
		SetCameraViewA(GetCameraLocation(), getFrameTime(12));
		
		// Stamina
		GET_STATS->AddStamina(
			-2.f / getHeroStatsComp()->getTeleportCost(), 
			getFrameTime(4), 
			false, 
			EIchigoState::Ichi_Attack_FW
		);

		SetBlockingAttack(EBlockType::Forward, getFrameTime(4), BLOCK_DURATION);
		DangerN(getFrameTime(5), EDangerType::MeleeAttack);
		Combo(getFrameTime(11));

		DashStartLoc = GetActorLocation();
		PlayTimeline(this, b_AttackDashCurve, "b_AttackDash", false);
	}

	void AIchigo::b_Attack_FW_Slash()
	{
		if (!GET_STATS->checkStamina(2.f / getHeroStatsComp()->getTeleportCost(), false))
		{
			NotEnoughStamina();
			if (CheckState(EIchigoState::Ichi_Attack_FW_Slash))
			{
				b_Attack_FW_End();
			}
			return;
		}

		if (CheckState(EIchigoState::Ichi_Attack_FW))
		{
			b_SlashLocation = GetActorLocation();
		}

		FState nState;
		nState.Animation = "Hide";
		nState.EndState = false;
		nState.Rotate = false;
		nState.State = EIchigoState::Ichi_Attack_FW_Slash;
		NewState(nState);

		// Stamina
		GET_STATS->AddStamina(-2.f / getHeroStatsComp()->getTeleportCost());

		// Camera Behaviour
		SetCameraViewA(GetCameraViewPosition(), 0.5f);

		getShadow()->HideShadow();
	

		SpawnHelper("b_Attack_FW_Slash", 0.1f);
		SpawnHelper("b_Attack_FW_Slash", 0.15f);
		SpawnHelper("b_Attack_FW_Slash", 0.2f);
		SpawnHelper("b_Attack_FW_Slash", 0.25f);
		SpawnHelper("Teleport", 0.f, GetActorRotation());

		SetActorLocation(
			b_SlashLocation + FVector(120.f * ((IsLookingRight()) ? -1 : 1.f), 0.f, 0.f), true
		);

		// After Image
		for (int i = 0; i < 3; i++)
		{
			FVector nLoc(GetActorLocation());
			FAfterImageStruct nImg("Run", nLoc, true, 0);
			nImg.SetDistance(FMath::RandRange(80.f, 150.f), FRotator(0.f, FMath::RandRange(0.f, 360.f), 0.f));
			nImg.Rotation = (nImg.Location.X < nLoc.X) ? true : false;
			AfterImage->Create(nImg, i * 0.1f);
		}

		FAfterImageStruct nImg("Stand", GetActorLocation(), IsLookingRight(), 0);
		AfterImage->Create(nImg, 0.f);

		SetImmortality(0.35f);
		Combo(0.35f);
	}

	void AIchigo::b_Attack_FW_End()
	{
		getShadow()->ShowShadow();

		SetActorLocation(
			b_SlashLocation + FVector(200.f * ((IsLookingRight()) ? -1 : 1.f), 0.f, 0.f), true
		);
		SpawnHelper("Teleport", 0.f, GetActorRotation());

		EndStateDeferred(0.1f);
	}

	void AIchigo::b_Attack_B()
	{
		FState nState;
		nState.State = EIchigoState::Ichi_Attack_B;
		nState.Animation = "Attack_B";
		NewState(nState);

		AddImpulse(SP_VELOCITY, getFrameTime(4));
		SpawnHelper("b_Attack_B", getFrameTime(6));
		Combo(getFrameTime(11));

		SetBlockingAttack(EBlockType::Forward, getFrameTime(5), BLOCK_DURATION);
		DangerN(getFrameTime(6), EDangerType::MeleeAttack);
	}

	void AIchigo::b_Attack_Air()
	{
		FState nState;
		nState.State = EIchigoState::Ichi_Attack_Air;
		nState.Animation = "AttackAir";
		NewState(nState);

		AddImpulse(FVector(GetUnitVelocity().X, GetUnitVelocity().Y, 200.f), getFrameTime(4));
		SpawnHelper("b_AttackAir", getFrameTime(4));
		Combo(getFrameTime(12));

		SetBlockingAttack(EBlockType::Forward, getFrameTime(4), BLOCK_DURATION);
		DangerN(getFrameTime(6), EDangerType::MeleeAttack);
	}

	// Ichi_Bankai Skills

	void AIchigo::b_Getsuga()
	{

		if (!getHeroStatsComp()->CheckSkill("Getsuga"))
			return;

		if (!getHeroStatsComp()->checkStamina(-(GETSUGA_COST)))
		{
			NotEnoughStamina();
			return;
		}

		if (!getHeroStatsComp()->checkPower(-(GETSUGA_COST)))
		{
			NotEnoughPower();
			return;
		}

		FState nState;
		nState.State = EIchigoState::Ichi_GetsugaFW;
		nState.Animation = "Getsuga";
		NewState(nState);

		AddImpulse(MoveVector * 300.f, 0.f);

		SpawnHelper("b_GetsugaEff", getFrameTime(2));
		SpawnHelper("b_Getsuga", getFrameTime(9));

		GET_STATS->AddStamina(GETSUGA_COST, AnimElemTime(9), true, EIchigoState::Ichi_GetsugaFW);

		SetBlockingAttack(EBlockType::Both, 0.f, getFrameTime(10));
		Combo(getFrameTime(15));
		SkillDisable();
	}

	void AIchigo::b_RExplosion()
	{
		if (!getHeroStatsComp()->CheckSkill("RExplosion"))
			return;

		if (!getHeroStatsComp()->checkStamina(-(EXPLOSION_COST)))
		{
			NotEnoughStamina();
			return;
		}

		if (!getHeroStatsComp()->checkPower(-(EXPLOSION_COST)))
		{
			NotEnoughPower();
			return;
		}

		FState nState;
		nState.State = EIchigoState::Ichi_RExplosion;
		nState.Animation = "RExplosion";
		NewState(nState);

		AddImpulse(MoveVector * 300.f, 0.f);
		SpawnHelper("b_RExplosion", getFrameTime(7));
		GET_STATS->AddStamina(EXPLOSION_COST, AnimElemTime(7), true, EIchigoState::Ichi_RExplosion);

		SetBlockingAttack(EBlockType::Both, 0.f, AnimElemTime(14));
		Combo(getFrameTime(16));
		SkillDisable();
	}

	void AIchigo::b_Shikai()
	{
		if (!getHeroStatsComp()->CheckSkill("Bankai"))
			return;

		FState nState;
		nState.State = EIchigoState::Ichi_Shikai;
		nState.Animation = "Shikai";
		NewState(nState);

		Shikai();
		Combo(getFrameTime(8));
		SetImmortality(AnimElemTime(4));
		GET_STATS->SetExpMultiplier(1.f);
		SkillDisable();
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

		} // Switch End
	}