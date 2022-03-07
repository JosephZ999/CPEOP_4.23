// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Sys/Interfaces/AIEvents.h"
#include "UnitAIBase.generated.h"

class USphereComponent;
class AUnitBase;

/**
 *
 */

UCLASS()
class CPEOP_API AUnitAIBase : public AAIController, public IAIEvents
{
	GENERATED_BODY()

public:
	AUnitAIBase();

private:
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess = "true"))
	USphereComponent* _SphereComp;

	UPROPERTY()
	AUnitBase* _Enemy;

	UPROPERTY()
	AUnitBase* _Ally;

	float _SearchEnemyStepSize;
	uint8 _SearchEnemyStepNum;

	FDangerArg	 _DangerInfo;
	FTimerHandle _DangerNullTimer;
	FTimerHandle _AITick;

protected:
	FORCEINLINE class AUnitBase* getEnemy() const { return _Enemy; }

	FVector getPawnLocation() const;
	FVector getEnemyLocation() const;
	FVector getEnemyLocation(float dist) const;

	/* Distance to Enemy in 3D space*/
	FORCEINLINE float getDistance() const
	{
		return FVector2D::Distance({getPawnLocation().X, getPawnLocation().Y}, // point A
			{getEnemyLocation().X, getEnemyLocation().Y});					   // point B
	}

	/* Distance to Enemy in 3D space*/
	FORCEINLINE float getDistance(FVector TargetPoint) const
	{
		return FVector2D::Distance({getPawnLocation().X, getPawnLocation().Y}, // point A
			{TargetPoint.X, TargetPoint.Y});								   // point B
	}

	/* Distance to Enemy X axis only */
	FORCEINLINE float getDistanceX() const { return FMath::Abs(getPawnLocation().X - getEnemyLocation().X); }

	/* Distance to Enemy Y axis only */
	FORCEINLINE float getDistanceY() const { return FMath::Abs(getPawnLocation().Y - getEnemyLocation().Y); }

	/* Distance to Enemy Z axis only*/
	FORCEINLINE float getDistanceZ() const { return FMath::Abs(getPawnLocation().Z - getEnemyLocation().Z); }

	FORCEINLINE bool isEnemyOnRight() { return getPawnLocation().X < getEnemyLocation().X; }

	FORCEINLINE bool isPointOnRight(FVector Point) { return getPawnLocation().X < Point.X; }

	/* Get Vector to enemy*/
	FVector getForwardVector() const;
	FVector getForwardVector(float addDistX) const;
	FVector getForwardVector(const FVector TargetPoint);
	FVector getForwardVector(const FVector& A, const FVector& B);

protected:
	// Searching Enemy
	bool  SearchEnemy(uint8 team);
	float GetSearchSize() const { return _SearchEnemyStepSize; }
	uint8 GetSearchStepNum() const { return _SearchEnemyStepNum; }
	void  SetSearchOptions(float StepRadius, uint8 SearchStepNum)
	{
		_SearchEnemyStepSize = StepRadius;
		_SearchEnemyStepNum	 = SearchStepNum;
	}

private:
	bool CanSearch = true;
	void EnableSearching();

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI Interface")
	void SetAIEnabled(bool Enable);

	UFUNCTION(BlueprintCallable, Category = AI)
	void Wait(float time);

	void ResumeAI();

	// Enemy
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI Interface")
	void SetEnemy(AUnitBase* ObjectRef);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = AI)
	AUnitBase* GetEnemy() const { return _Enemy; }

	void ClearEnemy() { _Enemy = nullptr; }

	// Ally
	AUnitBase* GetAlly();

	// Danger
	UFUNCTION(BlueprintNativeEvent, Category = "AI Interface") void OnDangerDetected(FDangerArg& DangerInfo);

	void ClearDangerInfo();

	const FDangerArg& GetDangerInfo() const { return _DangerInfo; }

	bool IsInsideADangerBox();

protected:
	virtual void BeginPlay() override;
	virtual void AIBody() {}
};
