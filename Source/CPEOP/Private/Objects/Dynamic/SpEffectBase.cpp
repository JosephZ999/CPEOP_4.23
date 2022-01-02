// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Dynamic/SpEffectBase.h"

// Sets default values
ASpEffectBase::ASpEffectBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpEffectBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpEffectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

