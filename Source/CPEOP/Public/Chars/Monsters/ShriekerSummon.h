// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chars/MonsterBase.h"
#include "ShriekerSummon.generated.h"

/**
 * 
 */
UCLASS()
class CPEOP_API AShriekerSummon : public AMonsterBase
{
	GENERATED_BODY()

public:
	AShriekerSummon();

private:
	virtual void EndState() override;
	virtual void AnimUpdate() override;
	
public:
	void LandAttack();
	void JumpAttack();
	void JumpTo(FVector forwardVector);
};
