// Fill out your copyright notice in the Description page of Project Settings.

#include "Objects/Dynamic/HitBoxBase.h"

#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
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

// Called when the game starts or when spawned
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

// Init // Init // Init // Init // Init // Init // Init // Init //
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

void AHitBoxBase::Attack(AUnitBase* Enemy)
{
	if (Enemy && ! Enemy->CheckTeam(OwnerCharacter->GetTeam()) && ! Enemy->IsFalling() && ! Enemy->CheckState(EBaseStates::Teleport))
	{
		bool fBehind{false};
		bool blocked{false};

		switch (ImpulseType)
		{
		case EImpulseType::Rotation:
		{
			fBehind = (FMath::IsNearlyZero(GetActorRotation().Yaw) && Enemy->IsLookingRight()) ||
					  (! FMath::IsNearlyZero(GetActorRotation().Yaw) && ! Enemy->IsLookingRight());
			break;
		}
		// case EImpulseType::Location: break;
		case EImpulseType::OwnerLocation:
		{
			fBehind = (Enemy->GetActorLocation().X > OwnerCharacter->GetActorLocation().X && Enemy->IsLookingRight()) ||
					  (Enemy->GetActorLocation().X < OwnerCharacter->GetActorLocation().X && ! Enemy->IsLookingRight());
			break;
		}
		}

		if (Enemy->ApplyDamage(OwnerCharacter, &Options, fBehind, blocked))
		{
			++HitCount;
			OnHit(HitCount, Enemy, blocked);
		}
	}
}

void AHitBoxBase::OnHit_Implementation(const int32& count, class AUnitBase* damagedUnit, bool attackBlocked) {}
