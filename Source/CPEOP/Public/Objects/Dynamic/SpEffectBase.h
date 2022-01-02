// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/Dynamic/Helper.h"
#include "SpEffectBase.generated.h"

UCLASS()
class CPEOP_API ASpEffectBase : public AHelper
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpEffectBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
