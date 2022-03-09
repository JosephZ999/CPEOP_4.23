// Fill out your copyright notice in the Description page of Project Settings.

#include "Chars/AI/UnitAIBase.h"
#include "Chars/UnitBase.h"
#include "Kismet/KismetMathLibrary.h"

#include "Components/SphereComponent.h"
#include "TimerManager.h"

#define DANGER_CLEAR_TIME	  0.5f
#define SEARCH_ENEMY_INTERVAL 0.5F

AUnitAIBase::AUnitAIBase()
{
	_SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	_SphereComp->SetupAttachment(RootComponent);
	_SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	_SphereComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	_SphereComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	bAttachToPawn		 = true;
	_SearchEnemyStepSize = 50.f;
	_SearchEnemyStepNum	 = 10;
}

FVector AUnitAIBase::getPawnLocation() const
{
	return GetPawn()->GetActorLocation();
}

FVector AUnitAIBase::getEnemyLocation() const
{
	return getEnemy()->GetActorLocation();
}

FVector AUnitAIBase::getEnemyLocation(float dist) const
{
	if (getPawnLocation().X > getEnemyLocation().X)
	{
		return getEnemyLocation() + FVector(dist, 1.f, 0.f);
	}
	else
	{
		return getEnemyLocation() - FVector(dist, 1.f, 0.f);
	}
}

FVector AUnitAIBase::getForwardVector() const
{
	FRotator lookRot = UKismetMathLibrary::FindLookAtRotation(getPawnLocation(), getEnemyLocation());
	return UKismetMathLibrary::GetForwardVector(lookRot);
}

FVector AUnitAIBase::getForwardVector(float addDistX) const
{
	FRotator lookRot = UKismetMathLibrary::FindLookAtRotation(
		getPawnLocation() * FVector(1.f, 1.f, 0.f), getEnemyLocation(addDistX) * FVector(1.f, 1.f, 0.f));
	return UKismetMathLibrary::GetForwardVector(lookRot);
}

FVector AUnitAIBase::getForwardVector(const FVector TargetPoint)
{
	return FVector(UKismetMathLibrary::FindLookAtRotation(getPawnLocation(), TargetPoint).Vector());
}

FVector AUnitAIBase::getForwardVector(const FVector& A, const FVector& B)
{
	return FVector(UKismetMathLibrary::FindLookAtRotation(A, B).Vector());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void AUnitAIBase::BeginPlay()
{
	Super::BeginPlay();
}

void AUnitAIBase::SetAIEnabled_Implementation(bool Enable)
{
	if (Enable)
	{
		GetWorldTimerManager().SetTimer(_AITick, this, &AUnitAIBase::AIBody, 0.05f, true);
	}
	else
	{
		GetWorldTimerManager().PauseTimer(_AITick);
	}
}

void AUnitAIBase::SetEnemy_Implementation(AUnitBase* ObjectRef)
{
	_Enemy = ObjectRef;
}

void AUnitAIBase::Wait(float time)
{
	GetWorldTimerManager().PauseTimer(_AITick);
	GetWorldTimerManager().SetTimer(_AITick, this, &AUnitAIBase::ResumeAI, FMath::Max(time, 0.1f));
}

void AUnitAIBase::ResumeAI()
{
	GetWorldTimerManager().SetTimer(_AITick, this, &AUnitAIBase::AIBody, 0.05f, true);
}

void AUnitAIBase::OnDangerDetected_Implementation(FDangerArg& DangerInfo)
{
	if (DangerInfo.Priority > _DangerInfo.Priority)
	{
		_DangerInfo = DangerInfo;
		SET_TIMER(_DangerNullTimer, this, &AUnitAIBase::ClearDangerInfo, DANGER_CLEAR_TIME);
	}
}

void AUnitAIBase::ClearDangerInfo()
{
	PAUSE_TIMER(_DangerNullTimer);
	_DangerInfo.Priority = EDangerPriority::None;
}

bool AUnitAIBase::IsInsideADangerBox()
{
	const FVector pawnLocation = getPawnLocation();
	return pawnLocation.X > _DangerInfo.StartLocation.X && pawnLocation.X < _DangerInfo.EndLocation.X &&
		   pawnLocation.Y > _DangerInfo.StartLocation.Y && pawnLocation.Y < _DangerInfo.EndLocation.Y;
}

bool AUnitAIBase::SearchEnemy(uint8 team)
{
	if (! CanSearch)
		return false;

	// Reload
	CanSearch = false;
	FTimerHandle nTimer;
	SET_TIMER(nTimer, this, &AUnitAIBase::EnableSearching, SEARCH_ENEMY_INTERVAL);

	_SphereComp->SetSphereRadius(0.f);
	_SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	TArray<AActor*> OverActors;
	for (uint8 i = 1; i <= _SearchEnemyStepNum; i++)
	{
		_SphereComp->SetSphereRadius(i * _SearchEnemyStepSize);
		_SphereComp->GetOverlappingActors(OverActors, AUnitBase::StaticClass());

		for (AActor* elem : OverActors)
		{
			AUnitBase* unit = Cast<AUnitBase>(elem);

			if (unit->IsDead() || elem == GetPawn())
				continue;

			if (unit->CheckTeam(team))
			{
				if (! GetAlly())
				{
					_Ally = unit;
				}
				continue;
			}
			else
			{
				_Enemy = unit;
				_SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				return true;
			}
		}
	}
	return false;
}

void AUnitAIBase::EnableSearching()
{
	CanSearch = true;
}

AUnitBase* AUnitAIBase::GetAlly()
{
	//...
	return (IsValid(_Ally) && ! _Ally->IsDead()) ? _Ally : nullptr;
}
