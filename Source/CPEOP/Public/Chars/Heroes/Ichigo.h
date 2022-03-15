// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chars/HeroBase.h"
#include "Chars/Components/AfterImageComponent.h"

#include "Ichigo.generated.h"

#define SHIKAI_NAME			 "Shikai"
#define BANKAI_NAME			 "Bankai"

/* Attack Options */
#define BASE_VELOCITY		 (MoveVector + GetActorForwardVector()) * 150
#define SP_VELOCITY			 (MoveVector + GetActorForwardVector()) * 200
#define BLOCK_DURATION		 cTime(0.2f)

// Skills
#define GETSUGA_COST		 -2.f
#define GETSUGA_TENSHOU_COST -4.f
#define EXPLOSION_COST		 -4.f
#define BANKAI_COST			 -3.f

/* */
UENUM()
enum EIchigoState
{
	Ichi_Intro = EBaseStates::LastIndex,
	Ichi_Attack_1,
	Ichi_Attack_2,
	Ichi_Attack_3,
	Ichi_Attack_4,
	Ichi_Attack_FW,
	Ichi_Attack_B,
	Ichi_Attack_Air,

	Ichi_Attack_FW_Slash,
	Ichi_Attack_FW_End,

	Ichi_SwordTwist,
	Ichi_SwordTwistLoop,
	Ichi_SwordTwistEnd,
	Ichi_SwordThrow,

	Ichi_GetsugaStart,
	Ichi_GetsugaFW,

	Ichi_RExplosion,

	Ichi_Shikai,
	Ichi_Bankai,

};

UCLASS()
class CPEOP_API AIchigo : public AHeroBase
{
	GENERATED_BODY()

public:
	AIchigo();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	class UAfterImageComponent* AfterImage;

	// Animations
	TMap<FName, class UPaperFlipbook*> ShikaiAnim;
	TMap<FName, class UPaperFlipbook*> BankaiAnim;

protected:
	virtual void Landed(const FHitResult& Hit) override;

public:
	// Input Actions
	virtual void Attack() override;
	virtual void AttackBack() override;
	virtual void AttackForward() override;
	virtual void AttackDown() override;
	virtual void ComboI() override;

	FORCEINLINE void sh_InputA();
	FORCEINLINE void sh_InputB();
	FORCEINLINE void sh_InputFW();
	FORCEINLINE void sh_InputD();

	FORCEINLINE void b_InputA();
	FORCEINLINE void b_InputB();
	FORCEINLINE void b_InputFW();
	FORCEINLINE void b_InputD();

	FORCEINLINE void ShikaiComboI();
	FORCEINLINE void BankaiComboI();

	void Shikai()
	{
		ChangeForm(SHIKAI_NAME);
		AnimData = &ShikaiAnim;
	}
	void Bankai()
	{
		ChangeForm(BANKAI_NAME);
		AnimData = &BankaiAnim;
	}

	// Shikai
	void sh_Attack_1();
	void sh_Attack_2();
	void sh_Attack_FW();
	void sh_Attack_B();
	void sh_Attack_Air();

	void sh_GetsugaFW();
	void sh_GetsugaB();
	void sh_RExplosion();

	void		 sh_SwordTwist();
	void		 sh_SwordTwistLoop();
	void		 sh_SwordTwistEnd();
	FTimerHandle sh_STwistEndTimer;
	void		 sh_SwordThrow();

	void sh_GetsugaStart();
	void sh_GetsugaSlash();

	void sh_Bankai();

	// Bankai
	void	b_Attack_1();
	void	b_Attack_2();
	void	b_Attack_3();
	void	b_Attack_4();
	void	b_Attack_FW();
	void	b_Attack_FW_Slash();
	FVector b_SlashLocation;
	void	b_Attack_FW_End();
	void	b_Attack_B();
	void	b_Attack_Air();

	void b_Getsuga();
	void b_GetsugaFW();
	void b_GetsugaB();
	void b_RExplosion();

	void b_Shikai();

	UPROPERTY(BlueprintReadOnly)
	bool IsUsedBankai{false};

	// Timeline Movement // Dash
protected:
	UFUNCTION()
	void b_AttackDash(float value); // Timeline
private:
	UCurveFloat* b_AttackDashCurve;
	FVector		 DashStartLoc;
};
