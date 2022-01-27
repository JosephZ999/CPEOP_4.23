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

	FState nState;

	switch (GetState())
	{
	case EBaseStates::Jumping: { NewState(nState); break; }
	case EBaseStates::JumpLand: { NewState(nState); break; }
	} // Switch End

	UpdateAnim();
}

void AHollow_01::UpdateAnim()
{
	switch (GetState())
	{
	case EBaseStates::Stand:
	{
		FVector2D Speed = { GetVelocity().X, GetVelocity().Y };
		
		if (GetCharacterMovement()->IsMovingOnGround() && !Speed.IsNearlyZero(10.f))
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
	FState nState;
	nState.State = EMonsterStates::M_Attack_1;
	nState.Animation = "Attack";
	NewState(nState);

	SpawnHelper("AttackSlash", AnimElemTime(AttackHelperFrame));

	float animLength = GetSprite()->GetFlipbookLength();
	AttackDuration(AnimElemTime(AttackSuccessFrame));
	EndStateDeferred(animLength);
}
