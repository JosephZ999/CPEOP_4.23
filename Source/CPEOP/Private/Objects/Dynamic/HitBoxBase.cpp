// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Dynamic/HitBoxBase.h"

#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AHitBoxBase::AHitBoxBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Type = EHelperType::HitBox;

	// RootComponent = SpriteComp;
	GetSprite()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetSprite()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetSprite()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetSprite()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	GetSprite()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetSprite()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);

	// Hit Box Options
	Options.damage		= 1.f;
	Options.critRate	= 1.f;

	LoopAnim = false;
	HitCount = 0;
	
	ImpulseType = EImpulseType::OwnerLocation;

}

// Called when the game starts or when spawned
void AHitBoxBase::BeginPlay()
{
	Super::BeginPlay();

	GetSprite()->SetLooping(LoopAnim);

	// End of animation
	if (!LoopAnim)
	{
		FTimerHandle timer;
		float animLength = GetSprite()->GetFlipbookLength() / CustomTimeDilation;
		GetWorldTimerManager().SetTimer(timer, this, &AHitBoxBase::OnAnimFinished, animLength);
	}

	OnActorBeginOverlap.AddDynamic(this, &AHitBoxBase::OnBeginOverlap);
}


// Init // Init // Init // Init // Init // Init // Init // Init //
void AHitBoxBase::Init(AUnitBase* ownerChar, float damage, float crit)
{
	OwnerCharacter = ownerChar;
	Options.damage = Options.damage * damage;
	Options.critRate = crit;
	
}

void AHitBoxBase::OnBeginOverlap(AActor * OverlappedActor, AActor * OtherActor)
{
	AUnitBase* HitUnit = Cast<AUnitBase>(OtherActor);
	if (HitUnit && !HitUnit->CheckTeam(OwnerCharacter->GetTeam()) && !HitUnit->IsFalling() && !HitUnit->CheckState(EBaseStates::Teleport))
	{
		++HitCount;
		OnHit(HitCount, HitUnit);

		bool fBehind{ false };

		switch (ImpulseType)
		{
		case EImpulseType::Rotation: 
		{
			fBehind =	(FMath::IsNearlyZero(GetActorRotation().Yaw) && HitUnit->IsLookingRight()) 
				||		(!FMath::IsNearlyZero(GetActorRotation().Yaw) && !HitUnit->IsLookingRight());
			break;
		}
		// case EImpulseType::Location: break;
		case EImpulseType::OwnerLocation:
		{
			fBehind =	(HitUnit->GetActorLocation().X > OwnerCharacter->GetActorLocation().X && HitUnit->IsLookingRight()) 
					||	(HitUnit->GetActorLocation().X < OwnerCharacter->GetActorLocation().X && !HitUnit->IsLookingRight());
			break;
		}

		}

		HitUnit->ApplyDamage(OwnerCharacter, &Options, fBehind);
	}
}

void AHitBoxBase::OnAnimFinished_Implementation()
{
	Destroy();
}

void AHitBoxBase::OnHit_Implementation(const int32& count, class AUnitBase* damagedUnit)
{
	
}