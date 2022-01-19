// Fill out your copyright notice in the Description page of Project Settings.


#include "Hollow_01.h"
#include "PaperFlipbook.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PaperFlipbookComponent.h"

AHollow_01::AHollow_01()
{
	AddAnimation(FName("Stand"),   nullptr);
	AddAnimation(FName("Walk"),    nullptr);
	AddAnimation(FName("Attack"),  nullptr);
	AddAnimation(FName("Hit"),     nullptr);

	InitHelper("AttackSlash");

	CanFall = false;
}

void AHollow_01::Tick(float delta)
{
	Super::Tick(delta);
	UpdateAnim();
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
		float Speed = GetVelocity().X + GetVelocity().Y;

		if (GetCharacterMovement()->IsWalking(), !FMath::IsNearlyEqual(Speed, 0.f, 10.f))
		{
			SetAnim(FName("Walk"), false);
		}
		else
		{
			SetAnim(FName("Stand"), false);
		}
		break;
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
