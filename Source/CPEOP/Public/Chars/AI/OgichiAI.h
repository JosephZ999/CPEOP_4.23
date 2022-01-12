// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chars/AI/UnitAIBase.h"
#include "OgichiAI.generated.h"


UCLASS()
class CPEOP_API AOgichiAI : public AUnitAIBase
{
	GENERATED_BODY()

private:
	class AOgichi* OgichiRef;

	// Teleport
	bool canTeleport{ true };
	FTimerHandle tpReloadTimer;
	void tpReload(float time);
	void tpReset();

// Actions
	/*
	FORCEINLINE void A_None();
	FORCEINLINE void A_SearchEnemy();
	FORCEINLINE void A_Move();
	FORCEINLINE void A_Attack();
	*/
protected:
	virtual void BeginPlay() override;
public:
	virtual void AIBody() override;
};
