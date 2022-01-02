// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UnitAIBase.generated.h"

/**
 * 
 */

UCLASS()
class CPEOP_API AUnitAIBase : public AAIController
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereComp;
	class AUnitBase* Enemy;
public:
	FORCEINLINE class AUnitBase* getEnemy() const	{ return Enemy;}

	FVector getPawnLocation()const;
	FVector getEnemyLocation()const;
	FVector getEnemyLocation(float dist)const;

	/* Distance to Enemy in 3D space*/
	FORCEINLINE float getDistance()const	
	{
		return FVector2D::Distance(
			{ getPawnLocation().X,	getPawnLocation().Y },		// point A
			{ getEnemyLocation().X,	getEnemyLocation().Y });	// point B
	}

	/* Distance to Enemy X axis only */
	FORCEINLINE float getDistanceX()const	
	{ 
		return FMath::Abs(getPawnLocation().X - getEnemyLocation().X);
	}

	/* Distance to Enemy Y axis only */
	FORCEINLINE float getDistanceY()const
	{
		return FMath::Abs(getPawnLocation().Y - getEnemyLocation().Y);
	}

	/* Distance to Enemy Z axis only*/
	FORCEINLINE float getDistanceZ()const
	{
		return FMath::Abs(getPawnLocation().Z - getEnemyLocation().Z);
	}

	FORCEINLINE bool isEnemyOnRight()
	{
		return getPawnLocation().X < getEnemyLocation().X;
	}

	/* Get Vector to enemy*/
	FVector getForwardVector()const;
	FVector getForwardVector(float addDistX)const;

public:
	AUnitAIBase();
protected: virtual void BeginPlay() override;

public:
	bool SearchEnemy(uint8 team);
	UFUNCTION(BlueprintCallable, Category = AI)
	void StartAI(float time=0.f);

	UFUNCTION(BlueprintCallable, Category = AI)
	void StopAI();

	UFUNCTION(BlueprintCallable, Category = AI)
	void Wait(float time);

	FTimerHandle AITick;
	virtual void AIBody() {}
};
