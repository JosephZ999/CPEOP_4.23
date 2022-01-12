// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UnitStatsBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPEOP_API UUnitStatsBase : public UActorComponent
{
	GENERATED_BODY()


protected:
	UPROPERTY(EditDefaultsOnly)
	int32 ExpForKill;

public:	
	// Sets default values for this component's properties
	UUnitStatsBase();


	UFUNCTION(BlueprintCallable)
	virtual float GetHealth() const		{ return 0.f; };
	UFUNCTION(BlueprintCallable)
	virtual float GetMaxHealth() const	{ return 0.f; };

	UFUNCTION(BlueprintCallable)
	virtual float GetDamage() const		{ return 0.f; };
	UFUNCTION(BlueprintCallable)
	virtual float GetCritRate() const	{ return 0.f; };
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Calculate and return damage value
	virtual float TakeDamage(float damage, bool blocked) { return 0; }

	UFUNCTION(BlueprintCallable)
	virtual void AddExp(int32 exp) {}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetExpForKill() { return ExpForKill; }

	// Stamina
public:
	UFUNCTION(BlueprintCallable)
	virtual void     AddStamina(float value, float time = 0.f, bool skill = false);
	virtual void     RestoreStamina();

protected:
	virtual bool     AddStaminaCondition()        { return false; }
	virtual float    RestoreStaminaDelay()        { return 0.f;   }
	virtual bool     RestoreStaminaCondition()    { return false; }

private: 
	void             AddStaminaDeffered();
	float            AddStaminaValue;
	bool             AddStaminaSkill;
	FTimerHandle     AddStaminaTimer;
	FTimerHandle     RestoreStaminaTimer;
};
