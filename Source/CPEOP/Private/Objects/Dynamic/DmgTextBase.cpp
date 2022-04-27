// Fill out your copyright notice in the Description page of Project Settings.

#include "DmgTextBase.h"

// Sets default values
ADmgTextBase::ADmgTextBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.DoNotCreateDefaultSubobject(AHelper::SpriteName))
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ADmgTextBase::BeginPlay()
{
	Super::BeginPlay();
}
