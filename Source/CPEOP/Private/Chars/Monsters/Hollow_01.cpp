// Fill out your copyright notice in the Description page of Project Settings.


#include "Hollow_01.h"
#include "PaperFlipbook.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PaperFlipbookComponent.h"

AHollow_01::AHollow_01()
{
	AnimData = &AnimList;

	AnimList.Add(FName("Stand"),   nullptr);
	AnimList.Add(FName("Walk"),    nullptr);
	AnimList.Add(FName("Attack"),  nullptr);
	AnimList.Add(FName("Hit"),     nullptr);

	InitHelper("AttackSlash");

	CanFall = false;
}

void AHollow_01::Tick(float delta)
{
	Super::Tick(delta);
}

void AHollow_01::EndState()
{
	Super::EndState();

	switch (getState())
	{
	case EBaseStates::Jumping: { NewState(EBaseStates::Stand, "None"); break; }
	case EBaseStates::JumpLand: { NewState(EBaseStates::Stand, "None"); break; }
	} // Switch End

	UpdateAnim();
}

void AHollow_01::UpdateAnim()
{
	switch (getState())
	{
	case EBaseStates::Stand:
	{
		SetAnim(FName("Stand"), false);
	}
	} // Switch End
}

void AHollow_01::Attack()
{
	NewState((int)EMonsterStates::Attack_1, "None");
	SetAnim("Attack", true);

	SpawnHelper("AttackSlash", AnimElemTime(AttackHelperFrame));

	float animLength = GetSprite()->GetFlipbookLength();
	AttackDuration(AnimElemTime(AttackSuccessFrame));
	EndStateDeferred(animLength);
}
