// Fill out your copyright notice in the Description page of Project Settings.

#include "Renji.h"

#define R_SHIKAI_NAME "A"
#define R_ANIM_LOC	  "Texture/Chars/Renji/FBook/"
#define R_HIT_LOC	  "Blueprint/Chars/Renji/Shikai/"

ARenji::ARenji()
{
	if (GetHeroStats())
	{
		InitForm(R_SHIKAI_NAME, {4, 3, 3});
	}

	AnimData = &AnimList;

	InitAnim("Stand", R_ANIM_LOC "Stand");
	InitAnim("Walk", R_ANIM_LOC "Walk");
	InitAnim("JumpStart", R_ANIM_LOC "Jump_Start");
	InitAnim("JumpUp", R_ANIM_LOC "Jump_Up");
	InitAnim("JumpHold", R_ANIM_LOC "Jump_Down");
	InitAnim("JumpDown", R_ANIM_LOC "Jump_Down");
	InitAnim("JumpLand", R_ANIM_LOC "Jump_Land");

	InitAnim("Hit", R_ANIM_LOC "Hit_1");
	InitAnim("FallHold", R_ANIM_LOC "Fall");
	InitAnim("FallUp", R_ANIM_LOC "Fall_Up");
	InitAnim("FallDown", R_ANIM_LOC "Fall_Down");
	InitAnim("StandUp", R_ANIM_LOC "Stand_Up");
	InitAnim("StandUpAir", R_ANIM_LOC "Stand_Up_Air");

	InitAnim("Block", R_ANIM_LOC "Guard");
	InitAnim("BlockAir", R_ANIM_LOC "Guard_Air");
}

void ARenji::BeginPlay()
{
	Super::BeginPlay();
	ChangeForm(R_SHIKAI_NAME);
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
