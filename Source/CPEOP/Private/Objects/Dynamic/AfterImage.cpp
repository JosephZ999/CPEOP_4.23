// Fill out your copyright notice in the Description page of Project Settings.

#include "AfterImage.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Classes/Curves/CurveLinearColor.h"

#define FADE_CURVE_TIMEL "CurveFloat'/Game/Blueprint/Objects/Dynamic/AfterImage/AfterImageFade.AfterImageFade'"
#define FADE_CURVE_COLOR "CurveLinearColor'/Game/Blueprint/Objects/Dynamic/AfterImage/AfterImageColor.AfterImageColor'"

AAfterImage::AAfterImage()
{
	PrimaryActorTick.bCanEverTick = true;
	Sprite						  = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sprite"));
	RootComponent				  = Sprite;
	Sprite->SetRelativeScale3D(FVector(1.f, 1.f, 1.1f));
	Sprite->SetTranslucentSortPriority(2);
	// Sprite->Stop();
	// Sprite->SetLooping(false);

	ConstructorHelpers::FObjectFinder<UCurveFloat> nCurveFloat(TEXT(FADE_CURVE_TIMEL));
	if (nCurveFloat.Succeeded())
	{
		FadeTimeLineCurve = nCurveFloat.Object;
	}

	ConstructorHelpers::FObjectFinder<UCurveLinearColor> nCurveColor(TEXT(FADE_CURVE_COLOR));
	if (nCurveColor.Succeeded())
	{
		FadeColorCurve = nCurveColor.Object;
	}
}

void AAfterImage::Init(UPaperFlipbook* Anim, int8 Frame, bool OneFrame, bool LoopAnim)
{
	Sprite->SetFlipbook(Anim);
	Sprite->SetPlaybackPositionInFrames(Frame, false);

	if (OneFrame)
	{
		Sprite->Stop();
	}
	else
	{
		Sprite->SetLooping(LoopAnim);
		Sprite->Play();
	}
}

void AAfterImage::Tick(float delta)
{
	Super::Tick(delta);
	CurveTimeline.TickTimeline(delta);
}

void AAfterImage::Fade(float value)
{
	Sprite->SetSpriteColor(FadeColorCurve->GetLinearColorValue(value));

	if (CurveTimeline.GetPlaybackPosition() == CurveTimeline.GetTimelineLength())
	{
		CurveTimeline.Stop();
		Destroy();
	}
}

void AAfterImage::BeginPlay()
{
	Super::BeginPlay();

	if (FadeTimeLineCurve)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, "Fade");
		CurveTimeline.AddInterpFloat(FadeTimeLineCurve, TimelineProgress);
		CurveTimeline.SetLooping(false);
		CurveTimeline.PlayFromStart();
	}
}
