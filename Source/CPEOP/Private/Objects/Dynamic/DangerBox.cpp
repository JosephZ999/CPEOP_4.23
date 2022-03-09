// Fill out your copyright notice in the Description page of Project Settings.

#include "DangerBox.h"
#include "Sys/Interfaces/AIEvents.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Chars/UnitBase.h"

#define BOX_SIZE 25.f

// Sets default values
ADangerBox::ADangerBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	if (SceneRoot)
	{
		RootComponent = SceneRoot;
	}

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	if (Box)
	{
		Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Box->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		Box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		Box->SetupAttachment(SceneRoot);
		Box->SetBoxExtent(FVector(BOX_SIZE));
		Box->bHiddenInGame = false;
	}

	Plane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
	if (Plane)
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> nObject(TEXT("StaticMesh'/Game/Materials/Plane.Plane'"));
		if (nObject.Succeeded())
		{
			Plane->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Plane->SetStaticMesh(nObject.Object);
			Plane->SetupAttachment(SceneRoot);
			Plane->SetRelativeScale3D(FVector(0.5f));
		}

		// Init Materials

		ConstructorHelpers::FObjectFinder<UMaterialInterface> BoxMat(
			TEXT("MaterialInstanceConstant'/Game/Materials/DangerRegion/M_Danger_Box.M_Danger_Box'"));
		if (BoxMat.Succeeded())
		{
			_BoxMaterial = BoxMat.Object;
		}

		ConstructorHelpers::FObjectFinder<UMaterialInterface> SphereMat(
			TEXT("MaterialInstanceConstant'/Game/Materials/DangerRegion/M_Danger_Sphere.M_Danger_Sphere'"));
		if (SphereMat.Succeeded())
		{
			_SphereMaterial = SphereMat.Object;
		}

		Plane->SetMaterial(0, _BoxMaterial);
		Plane->TranslucencySortPriority = 3;

		// Init Fading Curve

		ConstructorHelpers::FObjectFinder<UCurveFloat> nCurve(TEXT("CurveFloat'/Game/Materials/DangerRegion/FadeIn.FadeIn'"));
		if (nCurve.Succeeded())
		{
			_CurveFadeIn = nCurve.Object;
		}

		ConstructorHelpers::FObjectFinder<UCurveFloat> nCurve2(TEXT("CurveFloat'/Game/Materials/DangerRegion/FadeOut.FadeOut'"));
		if (nCurve.Succeeded())
		{
			_CurveFadeOut = nCurve2.Object;
		}
	}

	OnActorBeginOverlap.AddDynamic(this, &ADangerBox::BeginOverlap);

	_IsFading  = false;
	_IsVisible = true;
	_Team	   = 0.f;
	_LifeTime  = 1.f;
	_Priority  = EDangerPriority::Normal;
}

void ADangerBox::Init(uint8 Team, float LifeTime, bool ShowRegion, EDangerPriority Priority)
{
	_Team	   = Team;
	_Priority  = Priority;
	_LifeTime  = LifeTime;
	_IsVisible = ShowRegion;

	if (_IsVisible)
	{
		switch (Priority)
		{
		case EDangerPriority::Explosion:
		{
			Plane->SetMaterial(0, _SphereMaterial);
			break;
		}
		} // Switch End
	}
	Plane->SetScalarParameterValueOnMaterials("Opacity", 0.f);
}

void ADangerBox::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(_LifeTime + 1.f);

	FTimerHandle FadeOutTimer;
	GetWorldTimerManager().SetTimer(FadeOutTimer, this, &ADangerBox::FadeOut, _LifeTime);

	// Set initial location
	FVector nLocation = Plane->GetComponentLocation();
	nLocation.Z		  = 1.f;
	Plane->SetWorldLocation(nLocation);

	if (_IsVisible && _CurveFadeIn)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, "TimeLine");
		_CTimeline.AddInterpFloat(_CurveFadeIn, TimelineProgress);
		_CTimeline.SetLooping(false);
		_CTimeline.PlayFromStart();
	}
}

void ADangerBox::Tick(float delta)
{
	Super::Tick(delta);
	_CTimeline.TickTimeline(delta);
}

void ADangerBox::BeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	AUnitBase* OtherUnit = Cast<AUnitBase>(OtherActor);

	if (! OtherUnit)
		return;
	if (OtherUnit->GetTeam() == _Team)
		return;

	FDangerArg nDanger;

	const FVector Size			= Box->GetScaledBoxExtent();
	const FVector OtherLocation = OtherActor->GetActorLocation();

	switch (_Priority)
	{
	case EDangerPriority::Explosion:
	{
		const FVector Location = GetActorLocation();
		nDanger.Position.X	   = FMath::Abs((Location.X - OtherLocation.X) / Size.X);
		nDanger.Position.Y	   = FMath::Abs((Location.Y - OtherLocation.Y) / Size.Y);
		nDanger.DangerPosition = Location;
		// nDanger.Size		   = {Size.X, Size.Y};
		break;
	}
	default:
	{
		bool	IsLookRight = GetActorForwardVector().X > 0.f;
		FVector Location	= GetActorLocation();
		Location.X += ((IsLookRight) ? (-Size.X) : Size.X);

		nDanger.Position.X	   = FMath::Abs((Location.X - OtherLocation.X) / (Size.X * 2));
		nDanger.Position.Y	   = FMath::Abs((Location.Y - OtherLocation.Y) / Size.Y);
		nDanger.DangerPosition = Location;
		// nDanger.Size		   = {Size.X, Size.Y};
	}
	} // End Switch

	nDanger.Priority = _Priority;

	const FVector Location = GetActorLocation();
	nDanger.StartLocation  = FVector2D(Location.X - Size.X, Location.Y - Size.Y);
	nDanger.EndLocation	   = FVector2D(Location.X + Size.X, Location.Y + Size.Y);

	IAIEvents::Execute_OnDangerDetected(OtherActor, nDanger);
}

void ADangerBox::OnOwnerStateChanged()
{
	if (Box)
	{
		Box->SetActive(false);
	}
	if (GetAttachParentActor())
	{
		DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
	}
}

void ADangerBox::FadeOut()
{
	if (_IsFading)
		return;

	_IsFading = true;

	if (Box)
	{
		Box->DestroyComponent();
	}

	if (_IsVisible && _CurveFadeOut)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, "TimeLine");
		_CTimeline.AddInterpFloat(_CurveFadeOut, TimelineProgress);
		_CTimeline.SetLooping(false);
		_CTimeline.PlayFromStart();
	}
	if (GetAttachParentActor())
	{
		DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
	}
	OnStartFade();
}

void ADangerBox::TimeLine(float Value)
{
	Plane->SetScalarParameterValueOnMaterials("Opacity", Value);
	//
}
