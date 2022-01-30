// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chars/AI/UnitAIBase.h"
#include "ShriekerSummonAI.generated.h"

/**
 * 
 */
UCLASS()
class CPEOP_API AShriekerSummonAI : public AUnitAIBase
{
	GENERATED_BODY()

	AShriekerSummonAI();

	class AShriekerSummon* OwnerRef;

	virtual void AIBody() override;

	// Attack 1 Conditions
	bool AttackSuccess;
	void AttackSuccessReset();
	FTimerHandle AttackTimer;

};
