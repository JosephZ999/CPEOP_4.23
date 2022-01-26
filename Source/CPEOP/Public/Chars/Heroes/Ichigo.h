// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chars/HeroBase.h"
#include "Ichigo.generated.h"

#define SHIKAI_NAME "Shikai"
#define BANKAI_NAME "Bankai"

// Skills
#define GETSUGA_COST -2.f
#define GETSUGA_TENSHOU_COST -4.f
#define EXPLOSION_COST -4.f

/* */
UENUM()
enum EIchigoState
{
	Intro = EBaseStates::LastIndex,
	Attack_1,
	Attack_2,
	Attack_3,
	Attack_4,
	Attack_FW,
	Attack_B,

	SwordTwist,
	SwordTwistLoop,
	SwordTwistEnd,
	GetsugaStart,
	GetsugaFW,

	RExplosion,

	Shikai,
	Bankai,

};

UCLASS()
class CPEOP_API AIchigo : public AHeroBase
{
	GENERATED_BODY()

public:
	AIchigo();

private:
	// Animations
	TMap<FName, class UPaperFlipbook*> ShikaiAnim;
	TMap<FName, class UPaperFlipbook*> BankaiAnim;
protected:
	virtual void BeginPlay() override;

public:
	// Input Actions
	virtual void Attack()			override;
	virtual void AttackBack()		override;
	virtual void AttackForward()	override;
	virtual void Btn_Bankai()       override;
	virtual void ComboI()			override;

	FORCEINLINE void ShikaiComboI();
	FORCEINLINE void BankaiComboI();

	// Actions
	void Shikai() { ChangeForm(SHIKAI_NAME);	AnimData = &ShikaiAnim; }
	void Bankai() {	ChangeForm(BANKAI_NAME);	AnimData = &BankaiAnim;	}

	// Shikai Actions
	void sh_Attack_1();
	void sh_Attack_2();
	void sh_AttackFW();
	void sh_AttackB();

	void sh_GetsugaFW();
	void sh_GetsugaB();
	void sh_RExplosion();

	void sh_SwordTwist();
	void sh_SwordTwistLoop();
	void sh_SwordTwistEnd();
	FTimerHandle sh_STwistEndTimer;


	void sh_GetsugaStart();
	void sh_GetsugaSlash();

	void sh_Bankai();

	// Bankai Actions
	void b_Attack_1();
	void b_Attack_2();
	void b_Attack_3();
	void b_Attack_4();
	void b_Attack_FW();
	void b_Attack_B();

	void b_RExplosion();

	void b_Shikai();

	// Bankai Dash Attack (FW)
protected:
	UFUNCTION()
	void b_AttackDash(float value);
private:
	UCurveFloat* b_AttackDashCurve;
	FVector DashStartLoc;
};
