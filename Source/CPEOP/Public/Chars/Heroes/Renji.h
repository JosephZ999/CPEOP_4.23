// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chars/HeroBase.h"
#include "Renji.generated.h"


/**
 * 
 */
UENUM()
enum class ERenjiShikai : uint8
{
	Intro = (uint8)EBaseStates::LastIndex,
	Attack_1,
	Attack_2,
	AttackFW,
	AttackB,

	LastIndex,
};


UCLASS()
class CPEOP_API ARenji : public AHeroBase
{
	GENERATED_BODY()

	ARenji();

	TMap<FName, class UPaperFlipbook*> AnimList;

protected:
	virtual void BeginPlay() override;

public:
	// Input Actions
	virtual void Attack()			override;
	virtual void AttackBack()		override;
	virtual void AttackForward()	override;
	virtual void ComboI()			override;


};
