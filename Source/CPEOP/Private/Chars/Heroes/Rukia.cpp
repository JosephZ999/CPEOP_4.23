// Fill out your copyright notice in the Description page of Project Settings.

#include "Rukia.h"
#include "Chars/AI/RukiaAI.h"
#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"

#define RUKIA_ANIM "Texture/Chars/Rukia/Anim/"
#define ATTACK_VEC 150.f

ARukia::ARukia()
{
	SetHeroName("Rukia");
	AIControllerClass = ARukiaAI::StaticClass();
	GetCapsuleComponent()->SetCapsuleSize(10.f, 24.f);
	GetSprite()->SetRelativeLocation(FVector(0.f, 0.f, -2.f));

	InitForm("Shikai", {2, 4, 4});

	// Init Animation
	AnimData = &ShikaiAnim;

	InitAnim("Stand", RUKIA_ANIM "Stand");
	InitAnim("Walk", RUKIA_ANIM "Run");
	InitAnim("JumpStart", RUKIA_ANIM "JumpStart");
	InitAnim("JumpUp", RUKIA_ANIM "JumpUp");
	InitAnim("JumpHold", RUKIA_ANIM "JumpHold");
	InitAnim("JumpDown", RUKIA_ANIM "JumpDown");
	InitAnim("JumpLand", RUKIA_ANIM "JumpLand");
	InitAnim("Hit", RUKIA_ANIM "Hit");
	InitAnim("FallHold", RUKIA_ANIM "FallHold");
	InitAnim("FallUp", RUKIA_ANIM "FallUp");
	InitAnim("FallDown", RUKIA_ANIM "FallDown");
	InitAnim("StandUp", RUKIA_ANIM "StandUp");
	InitAnim("StandUpAir", RUKIA_ANIM "JumpUp");
	InitAnim("Block", RUKIA_ANIM "Guard");
	InitAnim("BlockAir", RUKIA_ANIM "Guard");
	InitAnim("PowChargeStart", RUKIA_ANIM "Power");
	InitAnim("PowChargeLoop", RUKIA_ANIM "PowerLoop");
	InitAnim("PowChargeEnd", RUKIA_ANIM "Power");

	InitAnim("Attack_1", RUKIA_ANIM "Attack_1");
	InitAnim("Attack_2", RUKIA_ANIM "Attack_2");
	InitAnim("AttackAir_1", RUKIA_ANIM "AttackAir_1");
	InitAnim("AttackB", RUKIA_ANIM "AttackB");
	InitAnim("AttackFW_1", RUKIA_ANIM "AttackFW_1");
	InitAnim("AttackFW_2", RUKIA_ANIM "AttackFW_2");

	// Init Helpers
}

//----------------------------------------------// Inputs

void ARukia::Attack()
{
	switch (GetState())
	{
	case EBaseStates::Stand: Attack_1(); break;
	case EBaseStates::Jumping: AttackAir(); break;
	case ERukiaState::Ru_Attack_1:
		if (isComboTime())
		{
			Attack_2();
		}
		break;
	}
}

void ARukia::AttackBack()
{
	switch (GetState())
	{
	case EBaseStates::Stand: AttackB(); break;
	case ERukiaState::Ru_Attack_2:
		if (isComboTime())
		{
			AttackB();
		}
		break;
	case ERukiaState::Ru_AttackFW_2:
		if (isComboTime())
		{
			AttackB();
		}
		break;
	}
}

void ARukia::AttackForward()
{
	switch (GetState())
	{
	case EBaseStates::Stand: AttackFW_1(); break;
	case ERukiaState::Ru_AttackFW_1:
		if (isComboTime())
		{
			AttackFW_2();
		}
		break;
	}
}

void ARukia::AttackDown() {}

//----------------------------------------------// Combinations

void ARukia::ComboI()
{
	Super::ComboI();

	EComboKey key = getNextKey();

	switch (key)
	{
	case CK_None: break;
	case CK_Attack:
		switch (GetState())
		{
		case ERukiaState::Ru_Attack_1: Attack_2(); break;
		}
		break;
	case CK_AForward:
		switch (GetState())
		{
		case ERukiaState::Ru_AttackFW_1: AttackFW_2(); break;
		}
		break;
	case CK_ABackward:
		switch (GetState())
		{
		case ERukiaState::Ru_AttackFW_2: AttackB(); break;
		case ERukiaState::Ru_Attack_2: AttackB(); break;
		}
		break;
	case CK_Jump: break;
	case CK_Block: break;
	case CK_Dash: break;
	default: break;
	}
}

//----------------------------------------------// Actions Implementations

void ARukia::Attack_1()
{
	FState nState;
	nState.State	 = ERukiaState::Ru_Attack_1;
	nState.Animation = "Attack_1";
	NewState(nState);
	Combo(AnimElemTime(10));

	AddImpulse((GetMoveVector() + GetActorForwardVector()) * ATTACK_VEC, AnimElemTime(4));
}

void ARukia::Attack_2()
{
	FState nState;
	nState.State	 = ERukiaState::Ru_Attack_2;
	nState.Animation = "Attack_2";
	NewState(nState);
	Combo(AnimElemTime(7));

	AddImpulse((GetMoveVector() + GetActorForwardVector()) * ATTACK_VEC);
}

void ARukia::AttackFW_1()
{
	FState nState;
	nState.State	 = ERukiaState::Ru_AttackFW_1;
	nState.Animation = "AttackFW_1";
	NewState(nState);
	Combo(AnimElemTime(14));

	AddImpulse((GetMoveVector() + GetActorForwardVector()) * ATTACK_VEC * 2.f, AnimElemTime(7));
}

void ARukia::AttackFW_2()
{
	FState nState;
	nState.State	 = ERukiaState::Ru_AttackFW_2;
	nState.Animation = "AttackFW_2";
	NewState(nState);
	Combo(AnimElemTime(9));

	AddImpulse((GetMoveVector() + GetActorForwardVector()) * ATTACK_VEC);
}

void ARukia::AttackB()
{
	FState nState;
	nState.State	 = ERukiaState::Ru_AttackB;
	nState.Animation = "AttackB";
	NewState(nState);

	AddImpulse((GetMoveVector() + GetActorForwardVector()) * ATTACK_VEC * 0.8f, AnimElemTime(4));
}

void ARukia::AttackAir()
{
	FState nState;
	nState.State	 = ERukiaState::Ru_AttackAir_1;
	nState.Animation = "AttackAir_1";
	NewState(nState);

	AddImpulse(FVector(GetUnitVelocity().X, GetUnitVelocity().Y, 200.f), AnimElemTime(4));
}
