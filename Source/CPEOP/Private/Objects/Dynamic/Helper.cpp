// Fill out your copyright notice in the Description page of Project Settings.

#include "Helper.h"
#include "PaperFlipbookComponent.h"
#include "Components/SceneComponent.h"

FName AHelper::SpriteName(TEXT("SpriteComp"));

// Sets default values
AHelper::AHelper(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;

	Sprite = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(SpriteName);
	if (Sprite)
	{
		Sprite->SetupAttachment(RootComp);
		Sprite->SetTranslucentSortPriority(2);
	}

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

void AHelper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("AAA--------------------"));
}
