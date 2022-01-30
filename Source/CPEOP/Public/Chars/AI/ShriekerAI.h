// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chars/AI/UnitAIBase.h"
#include "ShriekerAI.generated.h"

/**
 * 
 */

UENUM()
enum class EActionType : uint8
{
	GetDistance,
	Attack,
	Summon,
};

UCLASS()
class CPEOP_API AShriekerAI : public AUnitAIBase
{
	GENERATED_BODY()

public:
	AShriekerAI();

private:
	class AShrieker* OwnerRef;
	EActionType Action;
	FVector DesiredLocation;
	float DistanceMax;
	float DistanceMin;
	float AttackDistance;

	// Move Away
	float MoveAwayProgress;
	bool CanOwnerMoveAway() { return MoveAwayProgress < 10.f; }
	bool IsMovingAway;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsSummoning() { return Action == EActionType::Summon; }


private:
	virtual void AIBody() override;
	FORCEINLINE void StopMoving();

	FORCEINLINE void ActionMode();
	FORCEINLINE void GetDistanceMode();
	FORCEINLINE void Summon();


};
