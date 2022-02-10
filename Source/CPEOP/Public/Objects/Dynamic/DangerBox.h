// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "DangerBox.generated.h"

//class USceneComponent;
//class UBoxComponent;
//enum EDangerPriority;

UCLASS()
class CPEOP_API ADangerBox : public AActor
{
	GENERATED_BODY()

	// Components Name
	static FName RootCompName;
	static FName BoxCompName;

private:
	UPROPERTY(Category = Component, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* SceneRoot;

	UPROPERTY(Category = Component, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* Box;

	enum EDangerPriority _Priority;
	uint8 _Team;
	float _LifeTime;

public:	
	// Sets default values for this actor's properties
	ADangerBox();

	UFUNCTION()
	void BeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void Init(uint8 Team, float LifeTime, enum EDangerPriority Priority);
};

