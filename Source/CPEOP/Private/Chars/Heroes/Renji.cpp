// Fill out your copyright notice in the Description page of Project Settings.


#include "Renji.h"

ARenji::ARenji()
{
	if (getHeroStatsComp())
	{
		InitForm(SHIKAI_NAME, { 4, 3, 3});
	}

	AnimData = &AnimList;


	AddAnimation("Stand",       ANIM_LOC "Stand");
	AddAnimation("Walk",        ANIM_LOC "Walk");
	AddAnimation("JumpStart",   ANIM_LOC "Jump_Start");
	AddAnimation("JumpUp",      ANIM_LOC "Jump_Up");
	AddAnimation("JumpHold",    ANIM_LOC "Jump_Down");
	AddAnimation("JumpDown",    ANIM_LOC "Jump_Down");
	AddAnimation("JumpLand",    ANIM_LOC "Jump_Land");

	AddAnimation("Hit",         ANIM_LOC "Hit_1");
	AddAnimation("FallHold",    ANIM_LOC "Fall");
	AddAnimation("FallUp",      ANIM_LOC "Fall_Up");
	AddAnimation("FallDown",    ANIM_LOC "Fall_Down");
	AddAnimation("StandUp",     ANIM_LOC "Stand_Up");
	AddAnimation("StandUpAir",  ANIM_LOC "Stand_Up_Air");

	AddAnimation("Block",       ANIM_LOC "Guard");
	AddAnimation("BlockAir",    ANIM_LOC "Guard_Air");
}

void ARenji::BeginPlay()
{
	Super::BeginPlay();
	ChangeForm(SHIKAI_NAME);
}


//---------------------------------------------// Inputs
	void ARenji::Attack()
	{
		Super::Attack();

	}

	void ARenji::AttackBack()
	{
		Super::AttackBack();
	}

	void ARenji::AttackForward()
	{
		Super::AttackForward();
	}

//---------------------------------------------// Actions 
	
//---------------------------------------------// Combinations
	void ARenji::ComboI()
	{
		Super::ComboI();
	}
