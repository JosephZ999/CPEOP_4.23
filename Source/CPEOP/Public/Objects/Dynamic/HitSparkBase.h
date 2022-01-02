// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/Dynamic/Helper.h"
#include "HitSparkBase.generated.h"

UCLASS()
class CPEOP_API AHitSparkBase : public AHelper
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHitSparkBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:	virtual void BeginPlay()			override;
public:		virtual void Tick(float DeltaTime)	override;

	// Functions
public:
	virtual void Init(class UPaperFlipbook* anim, float rotation) override;
	void Destroy();
};
