// Fill out your copyright notice in the Description page of Project Settings.


#include "Chars/AI/UnitAIBase.h"
#include "Chars/UnitBase.h"
#include "Kismet/KismetMathLibrary.h"

#include "Components/SphereComponent.h"
#include "TimerManager.h"

AUnitAIBase::AUnitAIBase()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereComp->SetupAttachment(RootComponent);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	bAttachToPawn = true;
	SearchStepRadius = 50.f;
	SearchSteps = 10;
	
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
	FRotator lookRot = UKismetMathLibrary::FindLookAtRotation
	(
		getPawnLocation() * FVector(1.f, 1.f, 0.f),
		getEnemyLocation(addDistX) * FVector(1.f, 1.f, 0.f)
	);
	return  UKismetMathLibrary::GetForwardVector(lookRot);
}

FVector AUnitAIBase::getForwardVector(const FVector & A, const FVector & B)
{
	return FVector(UKismetMathLibrary::FindLookAtRotation(A, B).Vector());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void AUnitAIBase::BeginPlay()
{
	Super::BeginPlay();
}

void AUnitAIBase::StartAI(float timeDelay)
{
	GetWorldTimerManager().SetTimer(AITick, this, &AUnitAIBase::AIBody, 0.05f, true, timeDelay);
}

void AUnitAIBase::AIBody()
{

}

void AUnitAIBase::StopAI()
{
	GetWorldTimerManager().PauseTimer(AITick);
}

void AUnitAIBase::SetEnemy(AUnitBase * Unit)
{
	Enemy = Unit;
}

bool AUnitAIBase::SearchEnemy(uint8 team)
{
	SphereComp->SetSphereRadius(0.f);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	TArray<AActor*> OverActors;
	for (uint8 i = 1; i <= SearchSteps; i++)
	{
		SphereComp->SetSphereRadius(i * SearchStepRadius);
		SphereComp->GetOverlappingActors(OverActors, AUnitBase::StaticClass());

		for (const AActor* elem : OverActors)
		{
			AUnitBase* unit = (AUnitBase*)elem;
			if (elem != GetPawn() && !unit->CheckTeam(team))
			{
				Enemy = unit;
				SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				return true;
			}
		}

	}
	return false;
}

void AUnitAIBase::Wait(float time)
{
	GetWorldTimerManager().PauseTimer(AITick);
	StartAI(time);
}