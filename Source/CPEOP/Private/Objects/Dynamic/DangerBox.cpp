// Fill out your copyright notice in the Description page of Project Settings.


#include "DangerBox.h"
#include "Sys/Interfaces/AIEvents.h"


FName ADangerBox::RootCompName(TEXT("RootComp"));
FName ADangerBox::BoxCompName(TEXT("CollisionBox"));

// Sets default values
ADangerBox::ADangerBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(RootCompName);
	if (SceneRoot)
	{
		RootComponent = SceneRoot;
	}

	Box = CreateDefaultSubobject<UBoxComponent>(BoxCompName);
	if (Box)
	{
		Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Box->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		Box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		Box->SetupAttachment(SceneRoot);
		Box->SetRelativeLocation(FVector(32.f, 0.f, 0.f));
	}
	OnActorBeginOverlap.AddDynamic(this, &ADangerBox::BeginOverlap);
}

void ADangerBox::BeginOverlap(AActor * OverlappedActor, AActor * OtherActor)
{
	float MaxLocX = Box->GetScaledBoxExtent().X * 2.f;
	float MaxLocY = Box->GetScaledBoxExtent().Y * 2.f;

	float ALocX = GetActorLocation().X;
	float ALocY = GetActorLocation().Y - MaxLocY / 2.f;

	float BLocX = OtherActor->GetActorLocation().X;
	float BLocY = OtherActor->GetActorLocation().Y;

	FDangerArg nDanger;
	nDanger.Position.X = FMath::Abs((ALocX - BLocX) / MaxLocX);
	nDanger.Position.Y = FMath::Abs((ALocY - BLocY) / MaxLocY);
	nDanger.Size = { MaxLocX, MaxLocY };

	IAIEvents::Execute_OnDangerDetected(OtherActor, nDanger);
}
