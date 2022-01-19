// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chars/MonsterBase.h"
#include "Hollow_01.generated.h"

/**
 * 
 */
UCLASS()
class CPEOP_API AHollow_01 : public AMonsterBase
{
	GENERATED_BODY()

public:
	AHollow_01();


protected:
	virtual void Tick(float delta) override;
	virtual void EndState() override;
	virtual void Landed(const FHitResult& Hit) {}

public:
	virtual void Attack() override;

private:
	void UpdateAnim();

	UPROPERTY(EditDefaultsOnly, Category = "Attack Options")
	uint8 AttackHelperFrame;

	UPROPERTY(EditDefaultsOnly, Category = "Attack Options")
	uint8 AttackSuccessFrame;
};
