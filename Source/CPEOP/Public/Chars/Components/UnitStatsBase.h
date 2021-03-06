// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UnitStatsBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPEOP_API UUnitStatsBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUnitStatsBase();

	UFUNCTION(BlueprintCallable)
	virtual float getHealth() const		{ return 0.f; };
	UFUNCTION(BlueprintCallable)
	virtual float getMaxHealth() const	{ return 0.f; };

	UFUNCTION(BlueprintCallable)
	virtual float getDamage() const		{ return 0.f; };
	UFUNCTION(BlueprintCallable)
	virtual float getCritRate() const	{ return 0.f; };
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual float    TakeDamage(float damage, bool blocked) { return 0; }

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
