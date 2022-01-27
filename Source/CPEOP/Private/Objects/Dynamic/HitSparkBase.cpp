// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Dynamic/HitSparkBase.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "TimerManager.h"


// Sets default values
AHitSparkBase::AHitSparkBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GetSprite()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	GetSprite()->SetGenerateOverlapEvents(false);
	GetSprite()->SetLooping(false);
}

// Called when the game starts or when spawned
void AHitSparkBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHitSparkBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Functions
void AHitSparkBase::Init(UPaperFlipbook* anim, float rotation)
{
	GetSprite()->SetFlipbook(anim);
	GetSprite()->PlayFromStart();
	GetSprite()->SetRelativeRotation({ FMath::FRandRange(rotation - 10, rotation + 10), 0.f, 0.f } );

	// Destroy timer
	FTimerHandle dTimer;
	GetWorldTimerManager().SetTimer(dTimer, this, &AHitSparkBase::Destroy, GetSprite()->GetFlipbookLength());
}

void AHitSparkBase::Destroy()
{
	Super::Destroy();
}