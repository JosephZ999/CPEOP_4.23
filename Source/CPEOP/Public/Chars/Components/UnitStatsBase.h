// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UnitStatsBase.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CPEOP_API UUnitStatsBase : public UActorComponent
{
	GENERATED_BODY()

public:
	UUnitStatsBase();

protected:
	UPROPERTY(EditDefaultsOnly)
	int32 ExpForKill;

	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = 0, ClampMax = 100))
	uint8 Level = 0;

public:
	UFUNCTION(BlueprintCallable)
	virtual void SetLevel(uint8 NewLevel);

	UFUNCTION(BlueprintCallable)
	virtual float GetHealth() const { return 0.f; };

	UFUNCTION(BlueprintCallable)
	virtual float GetMaxHealth() const { return 0.f; };

	UFUNCTION(BlueprintCallable)
	virtual float GetDamage() const { return 0.f; };

	UFUNCTION(BlueprintCallable)
	virtual float GetCritRate() const { return 0.f; };

	UFUNCTION(BlueprintCallable)
	virtual void AddExp(int32 exp) {}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetExpForKill() { return ExpForKill; }

	virtual void Init() {}

	virtual float TakeDamage(float damage, float armorPiercing, bool blocked) { return 0; }

protected:
	virtual void BeginPlay() override;

	// Stamina
public:
	UFUNCTION(BlueprintCallable)
	virtual void AddStamina(float value, float time = 0.f, bool skill = false, int desiredState = -1);
	virtual void RestoreStamina();

protected:
	virtual bool  AddStaminaCondition() { return false; }
	virtual float RestoreStaminaDelay() { return 0.f; }
	virtual bool  RestoreStaminaCondition() { return false; }
	bool		  AddStaminaCanceled();

private:
	void		 AddStaminaDeffered();
	float		 AddStaminaValue;
	bool		 AddStaminaSkill;
	FTimerHandle AddStaminaTimer;
	FTimerHandle RestoreStaminaTimer;

	uint8 const* OwnerState;
	int8		 DesiredState{-1};
};
