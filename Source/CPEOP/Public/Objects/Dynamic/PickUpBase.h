// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUpBase.generated.h"

UCLASS(Blueprintable, Abstract)
class CPEOP_API APickUpBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickUpBase();

	UFUNCTION(BlueprintImplementableEvent)
	void Init(uint8 Level, AActor* Target);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
