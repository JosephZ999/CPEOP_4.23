// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShadowComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPEOP_API UShadowComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UShadowComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	class AActor* Owner;

	UPROPERTY(BlueprintReadOnly)
	class AActor* ShadowObj;

	class TSubclassOf<AActor> ShadowClass;


	UPROPERTY(EditDefaultsOnly)
	float Scale;

	void SetupOwnerforShadow();
	FTimerHandle nTimer;

	void ShowShadow();
	void HideShadow();
	void DestroyShadow();
};
