// Fill out your copyright notice in the Description page of Project Settings.

#include "HitBoxBase.h"

#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

AHitBoxBase::AHitBoxBase()
{
	Type = EHelperType::HitBox;

	// RootComponent = SpriteComp;
	GetSprite()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetSprite()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetSprite()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetSprite()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	GetSprite()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetSprite()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);

	// Hit Box Options
	Options.damage	 = 1.f;
	Options.critRate = 1.f;

	LoopAnim = false;
	HitCount = 0;

	ImpulseType = EImpulseType::OwnerLocation;

	RandEnemy	= false;
	AttackDelay = 0.05f;
}

void AHitBoxBase::BeginPlay()
{
	Super::BeginPlay();

	GetSprite()->SetLooping(LoopAnim);

	// End of animation
	if (! LoopAnim)
	{
		FTimerHandle timer;
		float		 animLength = GetSprite()->GetFlipbookLength() / CustomTimeDilation;
		SetLifeSpan(animLength);
	}

	OnActorBeginOverlap.AddDynamic(this, &AHitBoxBase::OnBeginOverlap);
}

void AHitBoxBase::Init(AUnitBase* ownerChar, float damage, float crit)
{
	OwnerCharacter	 = ownerChar;
	Options.damage	 = Options.damage * damage;
	Options.critRate = crit;
}

void AHitBoxBase::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	AUnitBase* HitUnit = Cast<AUnitBase>(OtherActor);
	if (RandEnemy && HitUnit)
	{
		if (! HitUnit->CheckTeam(OwnerCharacter->GetTeam()))
		{
			EnemiesInRange.Add(HitUnit);
			SET_TIMER(AttackTimer, this, &AHitBoxBase::AttackRandUnit, AttackDelay);
		}
	}
	else
	{
		Attack(HitUnit);
	}
}

void AHitBoxBase::AttackRandUnit()
{
	AUnitBase* nEnemy = EnemiesInRange[FMath::RandRange(0, EnemiesInRange.Num() - 1)];
	if (nEnemy)
	{
		Attack(nEnemy);
		SetActorEnableCollision(false);
	}
}

void AHitBoxBase::Attack(AUnitBase* _Enemy)
{
	if (_Enemy && ! _Enemy->CheckTeam(OwnerCharacter->GetTeam()) && ! _Enemy->IsFalling() && ! _Enemy->CheckState(EBaseStates::Teleport))
	{
		bool fBehind{false};
		bool blocked{false};

		switch (ImpulseType)
		{
		case EImpulseType::Rotation:
		{
			fBehind = (FMath::IsNearlyZero(GetActorRotation().Yaw) && _Enemy->IsLookingRight()) ||
					  (! FMath::IsNearlyZero(GetActorRotation().Yaw) && ! _Enemy->IsLookingRight());
			break;
		}
		// case EImpulseType::Location: break;
		case EImpulseType::OwnerLocation:
		{
			fBehind = (_Enemy->GetActorLocation().X > OwnerCharacter->GetActorLocation().X && _Enemy->IsLookingRight()) ||
					  (_Enemy->GetActorLocation().X < OwnerCharacter->GetActorLocation().X && ! _Enemy->IsLookingRight());
			break;
		}
		}

		if (_Enemy->ApplyDamage(OwnerCharacter, &Options, fBehind, blocked))
		{
			++HitCount;
			OnHit(HitCount, _Enemy, blocked);
		}
	}
}

void AHitBoxBase::OnHit_Implementation(const int32& count, class AUnitBase* damagedUnit, bool attackBlocked) {}
