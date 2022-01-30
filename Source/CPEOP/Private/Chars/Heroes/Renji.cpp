// Fill out your copyright notice in the Description page of Project Settings.


#include "Renji.h"

#define SHIKAI_NAME "A"
#define ANIM_LOC "Texture/Chars/Renji/FBook/"
#define HIT_LOC	"Blueprint/Chars/Renji/Shikai/"

ARenji::ARenji()
{
	if (getHeroStatsComp())
	{
		InitForm(SHIKAI_NAME, { 4, 3, 3});
	}

	AnimData = &AnimList;


	InitAnim("Stand",       ANIM_LOC "Stand");
	InitAnim("Walk",        ANIM_LOC "Walk");
	InitAnim("JumpStart",   ANIM_LOC "Jump_Start");
	InitAnim("JumpUp",      ANIM_LOC "Jump_Up");
	InitAnim("JumpHold",    ANIM_LOC "Jump_Down");
	InitAnim("JumpDown",    ANIM_LOC "Jump_Down");
	InitAnim("JumpLand",    ANIM_LOC "Jump_Land");

	InitAnim("Hit",         ANIM_LOC "Hit_1");
	InitAnim("FallHold",    ANIM_LOC "Fall");
	InitAnim("FallUp",      ANIM_LOC "Fall_Up");
	InitAnim("FallDown",    ANIM_LOC "Fall_Down");
	InitAnim("StandUp",     ANIM_LOC "Stand_Up");
	InitAnim("StandUpAir",  ANIM_LOC "Stand_Up_Air");

	InitAnim("Block",       ANIM_LOC "Guard");
	InitAnim("BlockAir",    ANIM_LOC "Guard_Air");
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
