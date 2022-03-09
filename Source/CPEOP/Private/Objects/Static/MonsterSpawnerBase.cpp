// Fill out your copyright notice in the Description page of Project Settings.

#include "Objects/Static/MonsterSpawnerBase.h"

// Sets default values
AMonsterSpawnerBase::AMonsterSpawnerBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMonsterSpawnerBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMonsterSpawnerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
