// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chars/HeroBase.h"
#include "Rukia.generated.h"

UENUM()
enum ERukiaState
{
	Ru_Intro = EBaseStates::LastIndex,

	Ru_Attack_1,
	Ru_Attack_2,
	Ru_AttackFW_1,
	Ru_AttackFW_2,
	Ru_AttackB,
	Ru_AttackAir_1,
	Ru_AttackAir_2,
};

/**
 *
 */
UCLASS()
class CPEOP_API ARukia : public AHeroBase
{
	GENERATED_BODY()

public:
	ARukia();

private:
	TMap<FName, UPaperFlipbook*> ShikaiAnim;

public:
	// Input Actions
	virtual void Attack() override;
	virtual void AttackBack() override;
	virtual void AttackForward() override;
	virtual void AttackDown() override;
	virtual void ComboI() override;

	void Attack_1();
	void Attack_2();
	void AttackFW_1();
	void AttackFW_2();
	void AttackB();
	void AttackAir();
};
