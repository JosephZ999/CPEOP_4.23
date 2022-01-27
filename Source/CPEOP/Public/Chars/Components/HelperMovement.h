// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HelperMovement.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPEOP_API UHelperMovement : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHelperMovement();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	float Speed;

	UPROPERTY(EditDefaultsOnly, AdvancedDisplay, Category = "Settings")
	FVector MoveVector;

	UPROPERTY(EditDefaultsOnly, AdvancedDisplay, Category = "Settings")
	bool AlwaysUpdateMoveVector;

private:
	AActor* Owner;
	bool	IsEnabled;
	FVector UpdatedVector;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetEnabled(bool value);

};
