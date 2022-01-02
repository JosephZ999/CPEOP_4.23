// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/Dynamic/Helper.h"
#include "DmgTextBase.generated.h"

UCLASS()
class CPEOP_API ADmgTextBase : public AHelper
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADmgTextBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	float Damage;
	bool Crit;

	UPROPERTY(BlueprintReadOnly)
	bool MoveRight;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void Init(float damage, bool crit, bool isMovingRight) override
	{
		Damage = damage;
		Crit = crit;
		MoveRight = isMovingRight;
	}
	UFUNCTION(BlueprintCallable)
	void getOptions(float& dmg, bool& crit)const
	{
		dmg = Damage;
		crit = Crit;
	}
};
