// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "DangerBox.generated.h"

//class USceneComponent;
//class UBoxComponent;

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

public:	
	// Sets default values for this actor's properties
	ADangerBox();

	UFUNCTION()
	void BeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

};
