// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chars/AI/UnitAIBase.h"
#include "HollowAI_1.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EHollowType : uint8
{
	Default,
	Ranged,
};

UCLASS()
class CPEOP_API AHollowAI_1 : public AUnitAIBase
{
	GENERATED_BODY()

private:
	class AMonsterBase* OwnerRef;

	EHollowType HType;
	float MinDistX;
	float MaxDistX;

public:
	UFUNCTION(BlueprintCallable)
	void InitParams(EHollowType type, float minDistX, float maxDistX);

private:
	virtual void AIBody() override;

	FORCEINLINE void AITypeDef();
	FORCEINLINE void AITypeRanged();
};
