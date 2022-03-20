// Fill out your copyright notice in the Description page of Project Settings.

#include "Chars/UnitBase.h"

#include "Objects/Dynamic/Helper.h"
#include "Objects/Dynamic/HitSparkBase.h"
#include "Objects/Dynamic/DangerBox.h"
#include "Chars/Components/UnitStatsBase.h"
#include "Chars/Components/ShadowComponent.h"
#include "Sys/MyFunctionLibrary.h"
#include "sys/Interfaces/GameIns.h"

#include "chars/AI/UnitAIBase.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"

// Settings
#define HIT_TIME 0.2f

AUnitBase::AUnitBase()
{
	GetCapsuleComponent()->InitCapsuleSize(12.0f, 28.0f);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	GetSprite()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->BrakingFrictionFactor = 1.f;
	GetCharacterMovement()->GroundFriction		  = 1.f;
	GetCharacterMovement()->MaxWalkSpeed		  = 150.f;
	GetCharacterMovement()->GravityScale		  = 1.2f;

	AIControllerClass = AUnitAIBase::StaticClass();
	AutoPossessAI	  = EAutoPossessAI::PlacedInWorldOrSpawned;
	DangerNoticeType  = EDangerType::None;
	Dead			  = false;

	ShadowComp = CreateDefaultSubobject<UShadowComponent>(TEXT("Shadow"));

	InitHelper("BlockSpark", "Blueprint/Objects/Dynamic/BlockSpark");
}

void AUnitBase::BeginPlay()
{
	Super::BeginPlay();

	const FVector spriteScale = GetSprite()->GetComponentScale();
	GetSprite()->SetRelativeScale3D(FVector(spriteScale.X, spriteScale.Y, spriteScale.Z * 1.1f));
	EndStateDeferred(0.1f);
}

void AUnitBase::Tick(float delta)
{
	Super::Tick(delta);
	if (! IsDead())
	{
		Move();
	}
}

bool AUnitBase::IsOnGround() const
{
	return GetMovementComponent()->IsMovingOnGround();
}

// AI

void AUnitBase::SetEnemy_Implementation(AUnitBase* ObjectRef)
{
	if (GetController() && GetController()->GetClass()->ImplementsInterface(UAIEvents::StaticClass()))
	{
		IAIEvents::Execute_SetEnemy(GetController(), ObjectRef);
	}
}

void AUnitBase::CreateADangerBox(EDangerPriority Priority, FDangerOptions& Options)
{
	if (Options.LifeTime <= 0.f)
		return;

	TSubclassOf<ADangerBox> nClass = IGameIns::Execute_GetDangerBoxClass(GetGameInstance());
	if (! nClass)
		return;

	if (Options.AttachToOwner)
	{
		Options.Location.X *= IsLookingRight() ? 1.f : -1.f;
		Options.Location += GetActorLocation();
	}

	FVector nLocation{Options.Location};

	FTransform	nT(GetActorRotation(), nLocation, Options.Scale);
	ADangerBox* box = GetWorld()->SpawnActorDeferred<ADangerBox>(nClass, nT);
	if (box)
	{
		box->Init(GetTeam(), cTime(Options.LifeTime), Options.ShowRadius, Priority);
		box->FinishSpawning(nT);

		OnStateChanged.AddUObject(box, &ADangerBox::OnOwnerStateChanged);

		if (Options.AttachToOwner)
		{
			box->AttachToActor(this,
				FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true),
				"None");
			box->SetActorLocation(nLocation);
		}
	}
}

void AUnitBase::OnDangerDetected_Implementation(FDangerArg& DangerInfo)
{
	if (IsValid(GetController()) && GetController()->GetClass()->ImplementsInterface(UAIEvents::StaticClass()))
	{
		IAIEvents::Execute_OnDangerDetected(GetController(), DangerInfo);
	}

	// UE_LOG(LogTemp, Warning, TEXT("UnitBase OnDangerDetected Arg: Pos X - %f, Pos Y - %f"), Arg1.Position.X, Arg1.Position.Y);
}

// Functions
void AUnitBase::FindHelper(FString objectPath, TSubclassOf<class AHelper>& Class)
{
	// Добавляю в конец classPath - .'название класса'
	if (objectPath.IsEmpty())
		return;

	FString ObjectName = UMyFunctionLibrary::FindObjectName(objectPath);
	FString finalPath  = "Class'/Game/" + objectPath + "." + ObjectName + "_C'";

	ConstructorHelpers::FClassFinder<AHelper> nObject((TEXT("%s"), *finalPath));
	if (nObject.Succeeded())
	{
		Class = nObject.Class;
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Can't find the class, path: %s "), *objectPath);
}

UPaperFlipbook* AUnitBase::FindAnim(FString objectPath)
{
	FString ObjectName = UMyFunctionLibrary::FindObjectName(objectPath);
	FString finalPath  = "PaperFlipbook'/Game/" + objectPath + "." + ObjectName + "'";

	ConstructorHelpers::FObjectFinder<UPaperFlipbook> nObject((TEXT("%s"), *finalPath));
	if (nObject.Succeeded())
	{
		return nObject.Object;
	}

	UE_LOG(LogTemp, Warning, TEXT("Cannot find the object, path: %s "), *objectPath);
	return nullptr;
}

float AUnitBase::getFrameTime(uint8 frame)
{
	return (frame / GetSprite()->GetFlipbookFramerate()) / CustomTimeDilation;
}

FVector& AUnitBase::GetUnitVelocity()
{
	return GetCharacterMovement()->Velocity;
}

void AUnitBase::SetImmortality(float duration)
{
	Immortal = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
	if (duration > 0.f)
	{
		SET_TIMER(ImmortalityTimer, this, &AUnitBase::DisableImmortality, duration);
	}
	else
	{
		Immortal = false;
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	}
}

void AUnitBase::DisableImmortality()
{
	Immortal = false;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
}

// Movement

void AUnitBase::Move()
{
	if (Control)
	{
		AddMovementInput(MoveVector, 1.f);
		SetRotation(IsMovingRight());
	}
}

void AUnitBase::SetMoveVector(FVector nVec)
{
	MoveVector = {nVec.X, nVec.Y, nVec.Z};
}

void AUnitBase::SetRotation(bool right, bool moveVec)
{
	if (! Controller)
		return;

	if (CheckState(EBaseStates::Fall))
		return;

	if (moveVec)
	{
		if (! FMath::IsNearlyEqual(MoveVector.X, 0.f, 0.1f))
		{
			FRotator nRot = (right) ? FRotator::ZeroRotator : FRotator(0.f, 180.f, 0.f);
			SetActorRotation(nRot);
			Controller->SetControlRotation(nRot);
		}
	}
	else
	{
		FRotator nRot = (right) ? FRotator::ZeroRotator : FRotator(0.f, 180.f, 0.f);
		SetActorRotation(nRot);
		Controller->SetControlRotation(nRot);
	}
}

void AUnitBase::AddImpulse(FVector impulse, float time, bool overrideXY, bool overrideZ)
{
	ImpulseVector	  = impulse;
	ImpulseOverrideXY = overrideXY;
	ImpulseOverrideZ  = overrideZ;

	if (time > 0.f)
	{
		PAUSE_TIMER(ImpulseTimer);
		SET_TIMER(ImpulseTimer, this, &AUnitBase::ImpulseDeferred, time);
	}
	else
	{
		ImpulseDeferred();
	}
}

void AUnitBase::ImpulseDeferred()
{
	LaunchCharacter(ImpulseVector, ImpulseOverrideXY, ImpulseOverrideZ);
}

void AUnitBase::EventJump()
{
	if (State == EBaseStates::Stand)
	{
		FState nState;
		nState.State	 = EBaseStates::JumpStart;
		nState.Animation = "JumpStart";
		nState.EndState	 = false;
		NewState(nState);

		FTimerHandle nTimer;
		if (GetAnim(nState.Animation))
		{
			SET_TIMER(nTimer, this, &AUnitBase::Jumping, AnimElemTime(GetAnim(nState.Animation)->GetNumFrames()));
		}
		else
		{
			Jumping();
		}
	}
	else if (State == EBaseStates::Fall)
	{
		StandUp();
	}
}

void AUnitBase::Jumping()
{
	if (State == EBaseStates::JumpStart)
	{
		FState nState;
		nState.State	= EBaseStates::Jumping;
		nState.EndState = false;
		NewState(nState);

		FVector jumpVector = MoveVector * GetCharacterMovement()->MaxWalkSpeed;
		jumpVector.Z	   = GetCharacterMovement()->JumpZVelocity;

		LaunchCharacter(jumpVector, true, true);
	}
}

void AUnitBase::Landed(const FHitResult& Hit)
{
	if (State == EBaseStates::Jumping || State == EBaseStates::Stand)
	{
		if (! GetAnim(FName("JumpLand")) || GetUnitVelocity().Z > -150.f)
		{
			FState nState;
			nState.State = EBaseStates::Stand;
			NewState(nState);
			return;
		}

		FState nState;
		nState.State	 = EBaseStates::JumpLand;
		nState.Animation = "JumpLand";
		NewState(nState);
	}
}

// Hit Box

void AUnitBase::InitHelper(FName name, FString classPath)
{
	TSubclassOf<AHelper> nClass;
	FindHelper(classPath, nClass);
	if (nClass)
	{
		HelpersData.Add(name, nClass);
	}
	else
	{
		HelpersData.Add(name, nullptr);
	}
}

void AUnitBase::SpawnHelperDeferred(FName name, float time, FRotator rotation, FVector scale)
{
	if (! (HelpersData.FindRef(name)))
		return;

	HelpersOrder.Add(FHelperInfo(State, name, rotation, scale, time));

	if (time > 0.f)
	{
		FTimerHandle timer;
		SET_TIMER(timer, this, &AUnitBase::HelperSort, time);
	}
	else
	{
		HelperSort();
	}
}

void AUnitBase::HelperSort()
{
	HelpersOrder.Sort();

	for (FHelperInfo& elem : HelpersOrder)
	{
		if (elem.name != "None")
		{
			HelperSpawning(elem);
			elem.name = FName();
			return;
		}
	}
}

void AUnitBase::HelperSpawning(FHelperInfo info)
{
	if (info.state != State)
		return;

	AHelper* nHelper =
		GetWorld()->SpawnActorDeferred<AHelper>(HelpersData.FindRef(info.name), FTransform(info.rotation, GetActorLocation(), info.scale));
	if (nHelper)
	{
		OnStateChanged.AddUObject(nHelper, &AHelper::OnOwnerStateChanged);

		// Sets default values
		if (nHelper->Type == EHelperType::HitBox)
		{
			nHelper->Init(this, GetUnitStats()->GetDamage(), GetUnitStats()->GetCritRate());
		}

		if (nHelper->bAttachRotation)
		{
			info.rotation.Yaw		   = (IsLookingRight()) ? 0.f : 180.f;
			nHelper->InitialLocation.X = (IsLookingRight()) ? nHelper->InitialLocation.X : nHelper->InitialLocation.X * -1.f;
		}

		nHelper->InitialLocation.Y += 1.f;
		nHelper->CustomTimeDilation = this->CustomTimeDilation;

		nHelper->FinishSpawning(FTransform(info.rotation, nHelper->InitialLocation + GetActorLocation(), info.scale));

		// Attaching
		if (nHelper->bAttachToParent)
		{
			nHelper->AttachToActor(this,
				FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true),
				"None");
			nHelper->SetActorLocation(nHelper->InitialLocation + GetActorLocation());
		}
	}
}

void AUnitBase::CreateSpark(uint8 index, FVector2D scale, float rotation)
{
	UPaperFlipbook* nAnimation = IGameIns::Execute_GetSparkAnimation(GetGameInstance(), index);
	if (! nAnimation)
		return;

	// Spawn Location
	FVector nLocation = GetActorLocation();
	nLocation.X += FMath::FRandRange(-15.f, 15.f);
	nLocation.Z += FMath::FRandRange(-15.f, 15.f);
	nLocation.Y += 1.f;

	// Spawn Transform
	FTransform nTransform{FRotator::ZeroRotator, nLocation, {scale.X, 1.f, scale.Y}};

	AHelper* nSpark = GetWorld()->SpawnActorDeferred<AHitSparkBase>(AHitSparkBase::StaticClass(), nTransform);
	if (nSpark)
	{
		// Init animation and rotation
		nSpark->Init(nAnimation, rotation);

		// Finish Spawning
		nSpark->FinishSpawning(nTransform);

		// Attach to this actor
		// nSpark->AttachToActor(this,	FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld,
		// EAttachmentRule::KeepWorld, true)); nSpark->SetActorLocation(nLocation);
	}
}

void AUnitBase::CreateDamageText(float damage, bool moveRight, bool crit)
{
	TSubclassOf<AHelper> nClass = IGameIns::Execute_GetDamageTextClass(GetGameInstance());

	if (! nClass)
		return;

	if (FMath::IsNearlyEqual(damage, 0.f))
	{
		crit = false;
	}

	if (! IGameIns::Execute_CanCreateDamageText(GetGameInstance(), crit))
		return;

	// Location
	FTransform nTransform{FRotator::ZeroRotator, GetActorLocation(), FVector::OneVector};

	// Spawning
	AHelper* nText = GetWorld()->SpawnActorDeferred<AHelper>(nClass, nTransform);
	if (nText)
	{
		nText->Init(damage, crit, moveRight);
		nText->FinishSpawning(nTransform);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Creating damage text failed"));
	}
}

// Taking Damage

bool AUnitBase::ApplyDamage(class AUnitBase* damageCauser, FHitOption* damageOption, bool fromBehind, bool& Blocked)
{
	if (State == EBaseStates::Fall || State == EBaseStates::Teleport || Dead)
		return false;

	bool block{false};
	bool crit{false};

	FVector impulse(damageOption->impulse.X, 0.f, damageOption->impulse.Y);
	impulse.X = ((fromBehind && ! IsLookingRight()) || (! fromBehind && IsLookingRight())) ? impulse.X * -1.f : impulse.X;

	if (IsBlocking())
	{
		block = ! fromBehind;
	}
	else
	{
		if (BlockAttackType != EBlockType::None)
		{
			switch (BlockAttackType)
			{
			case EBlockType::Forward:
			{
				block = ! fromBehind;
				break;
			}
			case EBlockType::Back:
			{
				block = fromBehind;
				break;
			}
			case EBlockType::Both:
			{
				block				  = true;
				damageOption->impulse = FVector2D(0.f, 0.f);
				break;
			}
			}
		}
	}

	if (block)
	{
		GetUnitStats()->AddStamina(0.05f);

		// Block Spark
		FRotator SparkRot = {FRotator::ZeroRotator};
		SparkRot.Yaw	  = (impulse.X > 0.f) ? 180.f : 0.f;
		SpawnHelperDeferred("BlockSpark", 0.f, SparkRot);
	}
	else
	{
		crit = damageOption->isCriticalDamage();

		// Spark
		CreateSpark(damageOption->sparkIndex, damageOption->sparkScale,
			(damageCauser->IsLookingRight()) ? damageOption->sparkRotation : damageOption->sparkRotation * -1.f);
	}

	// Taking Damage
	float damage;
	if (crit)
	{
		damage = GetUnitStats()->TakeDamage(damageOption->damage * 1.5f, damageOption->armorPiercing, block);
	}
	else
	{
		damage = GetUnitStats()->TakeDamage(damageOption->damage, damageOption->armorPiercing, block);
	}

	// Damage Text
	CreateDamageText(damage / damageCauser->GetUnitStats()->GetDamage(), damageCauser->IsLookingRight(), crit);

	// Change State
	if (! block)
	{
		Blocked = false;
		Dead	= GetUnitStats()->GetHealth() <= 0.f;

		if (damage > GetUnitStats()->GetMaxHealth() * 0.05f || Dead)
		{
			FState nState;
			nState.State	 = EBaseStates::Hit;
			nState.Animation = "Hit";
			nState.EndState	 = false;
			NewState(nState);

			DisableBlocking();
			EndStateDeferred(0.4f);
			AddImpulse(impulse, HIT_TIME);
		}

		if (Dead)
		{
			IGameIns::Execute_OnUnitKilled(GetGameInstance(), damageCauser, this);
			EventDead();
		}

		// Falling
		if (damageOption->fall || Dead)
		{
			if (CanFall)
			{
				FallDeferred(HIT_TIME);
			}
			SetRotation(impulse.X < 0.f, false);
			impulse.Z += 100.f;
			AddImpulse(impulse, HIT_TIME);
		}
	}
	else
	{
		Blocked = true;
		AddImpulse(impulse, HIT_TIME);
	}
	return true;
}

void AUnitBase::FallDeferred(float time)
{
	SET_TIMER(fallTimer, this, &AUnitBase::Fall, time);
}

void AUnitBase::Fall()
{
	if (State == EBaseStates::Hit)
	{
		FState nState;
		nState.State	 = EBaseStates::Fall;
		nState.Animation = "FallUp";
		nState.EndState	 = false;
		NewState(nState);

		EndStateDeferred(3.f);
	}
}

void AUnitBase::SetBlockingAttack(EBlockType type, float start, float end)
{
	nBlockT = type;

	if (start > 0.f)
	{
		SET_TIMER(blockTimer, this, &AUnitBase::SetBlockType, start);
	}
	else
	{
		SetBlockType();
	}

	if (start + end > 0.f)
	{
		SET_TIMER(blockEndTimer, this, &AUnitBase::DisableBlocking, start + end);
	}
	else
	{
		DisableBlocking();
	}
}

void AUnitBase::SetBlockType()
{
	BlockAttackType = nBlockT;
}

void AUnitBase::DisableBlocking()
{
	PAUSE_TIMER(blockTimer);
	PAUSE_TIMER(blockEndTimer);
	BlockAttackType = EBlockType::None;
}

void AUnitBase::StandUp()
{
	if (Dead)
		return;

	if (GetCharacterMovement()->IsMovingOnGround() && FMath::IsNearlyZero(GetVelocity().X + GetVelocity().Z))
	{
		FState nState;
		nState.State	 = EBaseStates::StandUp;
		nState.Animation = "StandUp";
		NewState(nState);
	}
	else
	{
		if (GetVelocity().Z > 20.f)
		{
			FState nState;
			nState.State	 = EBaseStates::StandUp;
			nState.Animation = "StandUpAir";
			NewState(nState);
		}
	}
}

// State Type

void AUnitBase::NewState(FState& state)
{
	State = state.State;
	UPaperFlipbook* nAnim{AnimData->FindRef(state.Animation)};
	if (nAnim)
	{
		Control = false;
		GetSprite()->SetFlipbook(nAnim);
		GetSprite()->SetLooping(! state.EndState);
		GetSprite()->SetPlaybackPositionInFrames(state.AnimationFrame, false);
		GetSprite()->Play();

		if (state.Rotate)
		{
			SetRotation(IsMovingRight());
		}

		if (state.EndState)
		{
			EndStateDeferred(getFrameTime(nAnim->GetNumFrames() - state.AnimationFrame));
		}
		else
		{
			PAUSE_TIMER(EndStateTimer);
		}
	}
	else
	{
		if (state.Animation == "Hide")
		{
			Control = false;
			GetSprite()->SetFlipbook(nullptr);

			if (state.EndState)
			{
				EndStateDeferred(getFrameTime(nAnim->GetNumFrames() - state.AnimationFrame));
			}
			else
			{
				PAUSE_TIMER(EndStateTimer);
			}
		}
		else
		{
			Control = true;
		}
	}
	OnStateChanged.Broadcast();
}

void AUnitBase::NewState(uint8 state)
{
	EndState();
	State	= state;
	Control = false;
}

void AUnitBase::EndStateDeferred(float time)
{
	if (time > 0.f)
	{
		SET_TIMER(EndStateTimer, this, &AUnitBase::EndState, time);
	}
	else
	{
		EndState();
	}
}
void AUnitBase::EndState()
{
	// На всякий случаи
	PAUSE_TIMER(EndStateTimer);
	PAUSE_TIMER(ImpulseTimer);
	GetSprite()->SetLooping(true);
	GetSprite()->Play();
	HelpersOrder.Empty();

	if (State == EBaseStates::Fall)
	{
		StandUp();
	}
	else
	{
		if (GetCharacterMovement()->IsMovingOnGround())
		{
			FState nState;
			nState.State = EBaseStates::Stand;
			NewState(nState);
		}
		else
		{
			FState nState;
			nState.State = EBaseStates::Jumping;
			NewState(nState);
		}
	}
}

// Animations

void AUnitBase::InitAnim(FName name, FString flipbookPath)
{
	if (AnimData)
	{
		UPaperFlipbook* nAnim{nullptr};
		nAnim = FindAnim(flipbookPath);
		if (nAnim)
		{
			AnimData->Add(name, nAnim);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Animation data not selected"));
	}
}

void AUnitBase::SetAnim(UPaperFlipbook* anim, bool playFromStart)
{
	if (anim)
	{
		GetSprite()->SetFlipbook(anim);
		if (playFromStart)
		{
			GetSprite()->PlayFromStart();
		}
	}
	else
	{
		GetSprite()->SetFlipbook(nullptr);
	}
}
void AUnitBase::SetAnim(FName name, bool playFromStart)
{
	UPaperFlipbook* nAnim = AnimData->FindRef(name);
	if (nAnim)
	{
		GetSprite()->SetFlipbook(nAnim);
		if (playFromStart)
		{
			GetSprite()->PlayFromStart();
		}
	}
}

UPaperFlipbook* AUnitBase::GetAnim(FName AnimName)
{
	return AnimData->FindRef(AnimName);
}

float AUnitBase::AnimElemTime(uint8 frame)
{
	return frame / GetSprite()->GetFlipbookFramerate();
}

// Attack Danger Notification

void AUnitBase::DangerN(float duration, EDangerType type)
{
	DangerNoticeType = type;
	SET_TIMER(DamageNTimer, this, &AUnitBase::DangerNFinish, duration);
}

void AUnitBase::DangerNFinish()
{
	DangerNoticeType = EDangerType::None;
}

//----------------------------------------------// Unit Interface

bool AUnitBase::IsAlive_Implementation()
{
	return ! IsDead();
}

uint8 AUnitBase::GetUnitTeam_Implementation()
{
	return GetTeam();
}
