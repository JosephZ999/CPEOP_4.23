// Fill out your copyright notice in the Description page of Project Settings.

#include "objects/Dynamic/Helper.h"
#include "PaperFlipbookComponent.h"
#include "Components/SceneComponent.h"

FName AHelper::SpriteName(TEXT("SpriteComp"));

// Sets default values
AHelper::AHelper(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;

	Sprite = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(SpriteName);
	if (Sprite)
	{
		Sprite->SetupAttachment(RootComp);
	}


 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	 PrimaryActorTick.bCanEverTick = true;

	 Type = EHelperType::None;
	 bAttachToParent = true;
	 bAttachRotation = true;
	 InitialLocation = FVector::ZeroVector;

}

// Called when the game starts or when spawned
void AHelper::BeginPlay()
{
	Super::BeginPlay();

}

