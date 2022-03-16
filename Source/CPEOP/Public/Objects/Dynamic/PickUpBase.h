// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUpBase.generated.h"

USTRUCT(BlueprintType)
struct FPickUpParams
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FVector Location;

	UPROPERTY(BlueprintReadWrite)
	uint8 Level;

	UPROPERTY(BlueprintReadWrite, Meta = (ClampMin = 1, ClampMax = 10))
	uint8 Amount;

	UPROPERTY(BlueprintReadWrite)
	float Chance;
};

UCLASS()
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
