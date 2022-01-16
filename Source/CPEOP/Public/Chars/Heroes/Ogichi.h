// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chars/HeroBase.h"
#include "Ogichi.generated.h"

#define SHIKAI_NAME "Shikai"
#define BANKAI_NAME "Bankai"

// Skills
#define GETSUGA_COST -1.5f
#define GETSUGA_TENSHOU_COST -3.f

/* */
UENUM()
enum class EOgichiShikai : uint8
{
	Intro = (uint8)EBaseStates::LastIndex,
	Attack_1,
	Attack_2,
	AttackFW,
	AttackB,
	SwordTwist,
	SwordTwistLoop,
	SwordTwistEnd,
	SwordThrow,

	Getsuga,

	Bankai,
	LastIndex,
};

UENUM()
enum class EOgichiBankai : uint8
{
	Intro = (uint8)EOgichiShikai::LastIndex,
	Attack_1,
	Attack_2,
	Attack_3,
	LastIndex,
};

UCLASS()
class CPEOP_API AOgichi : public AHeroBase
{
	GENERATED_BODY()

private:
	UPROPERTY()
	// Animations
	TMap<FName, class UPaperFlipbook*> ShikaiAnim;
	TMap<FName, class UPaperFlipbook*> BankaiAnim;
protected:
	virtual void BeginPlay() override;

public:
	AOgichi();

	// Input Actions
	virtual void Attack()			override;
	virtual void AttackBack()		override;
	virtual void AttackForward()	override;
	virtual void ComboI()			override;
	virtual void Btn_Bankai()		override;

	FORCEINLINE void ShikaiComboI();
	FORCEINLINE void BankaiComboI();

	// Actions
	void Shikai() { ChangeForm(SHIKAI_NAME);	AnimData = &ShikaiAnim; }
	void Bankai() { ChangeForm(BANKAI_NAME);	AnimData = &BankaiAnim; }

	// Shikai Actions
	void sh_Attack_1();
	void sh_Attack_2();
	void sh_AttackFW();
	void sh_AttackB();
	void sh_AttackB2();

	void sh_SwordTwist();
	void sh_SwordTwistLoop();
	void sh_SwordTwistEnd();
	FTimerHandle sh_STwistEndTimer;

	void sh_SwordThrow();

	void sh_Getsuga();

	void sh_Bankai();
};
