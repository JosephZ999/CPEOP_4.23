// Fill out your copyright notice in the Description page of Project Settings.


#include "Chars/MonsterBase.h"
#include "PaperFlipbookComponent.h"
#include "Chars/Components/ShadowComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#define APPEARANCE_TIME 0.5f
#define DEATH_TIME 1.f

AMonsterBase::AMonsterBase()
{
	AnimData = &AnimList;

	Stats = CreateDefaultSubobject<UMonsterStats>(TEXT("MonsterStats"));
	InitHelper("DeathEffect", "Blueprint/Objects/Dynamic/MonsterDeathEffect");
}

void AMonsterBase::BeginPlay()
{
	FHitOption a;
	a.damage = 3.f;
	a.sparkScale = { 1.f, 1.f };
	a.impulse = { 100.f, 50.f };
	a.sparkIndex = 3;
	
	ApplyDamage(this, &a, false );

	Super::BeginPlay();
	GetSprite()->SetVisibility(false);
	getShadow()->HideShadow();
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);

	FTimerHandle nTimer;
	SET_TIMER(nTimer, this, &AMonsterBase::Appearance, APPEARANCE_TIME);

	// Anim
	SET_TIMER(AnimUpdateTimer, this, &AMonsterBase::AnimUpdate, AnimUpdateRate, true);
}

bool AMonsterBase::IsOnGround()
{
	return GetCharacterMovement()->IsMovingOnGround();
}


// Animations

void AMonsterBase::AddAnimation(FName index, UPaperFlipbook* elem)
{
	AnimList.Add(index, elem);
}

UPaperFlipbook * AMonsterBase::GetAnimation(FName index)
{
	return nullptr;
}

//
void AMonsterBase::Appearance()
{
	GetSprite()->SetVisibility(true);
	getShadow()->ShowShadow();
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	StartAI();
}

void AMonsterBase::OnDamaged()
{
	if (IsDead())
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
		GetCharacterMovement()->Velocity = GetVelocity() / 2.f;
		GetCharacterMovement()->GravityScale *= 0.75f;
		
		SpawnHelper("DeathEffect", 0.f, FRotator(), FVector(DeathEffScale));
		FTimerHandle nTimer;
		SET_TIMER(nTimer, this, &AMonsterBase::Death, DEATH_TIME);
	}
}

void AMonsterBase::Death()
{
	getShadow()->DestroyShadow();
	Destroy();
}

// ------------------------------------------/ Attack
void AMonsterBase::Attack()
{
}

void AMonsterBase::AttackDuration(float Duration)
{
	if (Duration > 0.f)
	{
		SET_TIMER(AttackEndTimer, this, &AMonsterBase::AttackSuccess, Duration);
		IsAttacking = true;
	}
	else
	{
		IsAttacking = false;
	}
}

void AMonsterBase::AttackSuccess()
{
	IsAttacking = false;
}