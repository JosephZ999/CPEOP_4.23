// Fill out your copyright notice in the Description page of Project Settings.


#include "Chars/UnitBase.h"

#include "Objects/Dynamic/Helper.h"
#include "Objects/Dynamic/HitSparkBase.h"
#include "Sys/MyGameInstance.h"
#include "Chars/Components/UnitStatsBase.h"

#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Settings
#define HIT_TIME 0.2f

AUnitBase::AUnitBase()
{
	GetCapsuleComponent()->InitCapsuleSize(12.0f, 28.0f);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	GetSprite()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->BrakingFrictionFactor = 1.f;
	GetCharacterMovement()->GroundFriction = 1.f;
	GetCharacterMovement()->MaxWalkSpeed = 150.f;

	DangerNoticeType = EDangerType::None;
	Dead = false;
}

void AUnitBase::BeginPlay()
{
	Super::BeginPlay();
	
	GameInsRef = Cast<UMyGameInstance>(GetGameInstance());

	const FVector spriteScale = GetSprite()->GetComponentScale();
	GetSprite()->SetRelativeScale3D(FVector(spriteScale.X, spriteScale.Y, spriteScale.Z * 1.1f));
}

void AUnitBase::Tick(float delta)
{
	Super::Tick(delta);
	Move();
}

// Functions
	void AUnitBase::findObject(FString objectPath, TSubclassOf<class AHelper>& Class)
	{
		// �������� � ����� classPath - .'�������� ������'
		FString ObjectName;
		int32 charIndex = objectPath.Len();
		while (true)
		{
			charIndex--;
			if (objectPath[charIndex] == '/')
			{
				for (int i = ++charIndex; i < objectPath.Len(); i++)
				{
					ObjectName.AppendChar(objectPath[i]);
				}
				break;
			}
			else if (charIndex == 0)
			{
				for (int i = ++charIndex; i < objectPath.Len(); i++)
				{
					ObjectName.AppendChar(objectPath[i]);
				}
				break;
			}
		}

		FString finalPath;
		finalPath = "Class'/Game/" + objectPath + "." + ObjectName + "_C'";
		ConstructorHelpers::FClassFinder<AHelper> nObject((TEXT("%s"), *finalPath));
		if (nObject.Succeeded())
		{
			Class = nObject.Class;
			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("Cannot find the class, path: %s "), *objectPath);
	}

	UPaperFlipbook* AUnitBase::findObject(FString objectPath) 
	{
		// �������� � ����� classPath - .'�������� ������'
		FString ObjectName;
		int32 charIndex = objectPath.Len();
		while (true)
		{
			charIndex--;
			if (objectPath[charIndex] == '/')
			{
				for (int i = ++charIndex; i < objectPath.Len(); i++)
				{
					ObjectName.AppendChar(objectPath[i]);
				}
				break;
			}
			else if (charIndex == 0)
			{
				for (int i = ++charIndex; i < objectPath.Len(); i++)
				{
					ObjectName.AppendChar(objectPath[i]);
				}
				break;
			}
		}

		FString finalPath;

		finalPath = "PaperFlipbook'/Game/" + objectPath + "." + ObjectName + "'";
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
// Movement //===================================------------------------------
	void AUnitBase::Move()
	{
		if (Control)
		{
			AddMovementInput(MoveVector, 1.f);
			SetRotation(isMovingRight());
		}
	}

	void AUnitBase::SetMoveVector(FVector nVec)
	{
		MoveVector = { nVec.X, nVec.Y, 0.f };
	}

	void AUnitBase::SetRotation(bool right, bool moveVec)
	{
		if (moveVec)
		{
			if (!FMath::IsNearlyEqual(MoveVector.X, 0.f, 0.1f))
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

	void AUnitBase::AddImpulse(FVector2D impulse, float time)
	{
		ImpulseVector = { impulse.X + FMath::FRandRange(-20, 20), FMath::FRandRange(-5, 5), impulse.Y };
		if (time > 0.f)
		{
			SET_TIMER(ImpulseTimer, this, &AUnitBase::ImpulseDeferred, time);
		}
		else
		{
			ImpulseDeferred();
		}
	}

	void AUnitBase::AddImpulse(FVector impulse, float time)
	{
		ImpulseVector = impulse;
		if (time > 0.f)
		{
			SET_TIMER(ImpulseTimer, this, &AUnitBase::ImpulseDeferred, time);
		}
		else
		{
			ImpulseDeferred();
		}
	}

	void AUnitBase::ImpulseDeferred()
	{
		LaunchCharacter(ImpulseVector, true, true);
	}

	void AUnitBase::EventJump()
	{
		if (State == EBaseStates::Stand)
		{
			NewState(EBaseStates::JumpStart, "JumpStart", 0, false, false);
			FTimerHandle nTimer;
			SET_TIMER(nTimer, this, &AUnitBase::Jumping, cTime(0.2f));
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
			NewState(EBaseStates::Jumping, "None", 0, true, false);
			FVector jumpVector = MoveVector * GetCharacterMovement()->MaxWalkSpeed;
			jumpVector.Z = GetCharacterMovement()->JumpZVelocity;

			LaunchCharacter(jumpVector, true, true);
		}
	}

	void AUnitBase::Landed(const FHitResult & Hit)
	{
		if (State == EBaseStates::Jumping || State == EBaseStates::Stand)
		{
			NewState(EBaseStates::JumpLand, "JumpLand", 0, false, true);
		}
	}
// End Movement //===============================------------------------------


// Hit Box//=====================================------------------------------
	void AUnitBase::InitHelper(FName name, FString classPath)
	{
		TSubclassOf<AHelper> nClass;
		findObject(classPath, nClass);
		if (nClass)
		{
			HelpersData.Add(name, nClass);
		}
	}

	void AUnitBase::SpawnHelper(FName name, float time, FRotator rotation, FVector scale)
	{
		if (!(HelpersData.FindRef(name)))
			return;

		HelpersOrder.Add(FHelperInfo(State, name, rotation, scale, time));

		if (time > 0.f)
		{
			FTimerHandle timer;
			SET_TIMER(timer, this, &AUnitBase::HelperSort, time);
		}
		else { HelperSort(); }
	}

	void AUnitBase::HelperSort()
	{
		HelpersOrder.Sort();
		
		for (FHelperInfo& elem : HelpersOrder)
		{
			if (elem.name != "None")
			{
				HelperSpawning(elem, HelpersData.FindRef(elem.name));
				elem.name = FName();
				return;
			}
		}
	}

	void AUnitBase::HelperSpawning(FHelperInfo info, TSubclassOf<class AHelper> hitClass)
	{
		if (info.state != State)
			return;
		
		AHelper* nHelper = GetWorld()->SpawnActorDeferred<AHelper>(HelpersData.FindRef(info.name), FTransform(info.rotation, GetActorLocation() , info.scale));
		if (nHelper)
		{
			// Sets default values
			if (nHelper->Type == EHelperType::HitBox)
			{
				nHelper->Init(this, getStatsComp()->getDamage(), getStatsComp()->getCritRate());
			}
			
			if (nHelper->bAttachRotation)
			{
				info.rotation.Yaw	= (isLookingRight()) ? 0.f : 180.f;
				nHelper->InitialLocation.X = (isLookingRight()) ? nHelper->InitialLocation.X : nHelper->InitialLocation.X *-1.f;
			}
			nHelper->InitialLocation.Y += 1.f;

			UGameplayStatics::FinishSpawningActor(nHelper, FTransform(info.rotation, nHelper->InitialLocation + GetActorLocation(), info.scale));
			
			// Attaching
			if (nHelper->bAttachToParent)
			{
				nHelper->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true), "None" );
				nHelper->SetActorLocation(nHelper->InitialLocation + GetActorLocation());
			}
		}
	}
// End Hit Box//=================================------------------------------

// Taking Damage //==============================------------------------------
	void AUnitBase::ApplyDamage(class AUnitBase* damageCauser, FHitOption* damageOption, bool fromBehind)
	{
		if (State == EBaseStates::Fall || State == EBaseStates::Teleport)
			return;

		bool block  { false };
		bool crit   { false };
	
		FVector2D impulse = damageOption->impulse;
		impulse.X = ((fromBehind && !isLookingRight()) || (!fromBehind && isLookingRight())) ? impulse.X * -1.f : impulse.X;

		if (isBlockingAttack())
		{
			block = !fromBehind;
		}
		else
		{
			if (BlockAttackType != EBlockType::None)
			{
				switch (BlockAttackType)
				{
				case EBlockType::Forward:	{ block = !fromBehind; break; }
				case EBlockType::Back:		{ block = fromBehind; break; }
				case EBlockType::Both:		{ block = true; break; }
				}
			}
		}
		
		if (block)	{ getStatsComp()->AddStamina(0.05f); }
		else		
		{ 
			crit = damageOption->isCriticalDamage(); 
			
			// Spark
			CreateSpark(damageOption->sparkIndex, damageOption->sparkScale, (damageCauser->isLookingRight()) ? damageOption->sparkRotation : damageOption->sparkRotation * -1.f);
		}

		// Taking Damage
		float damage;
		if (crit)	{ damage = getStatsComp()->TakeDamage(damageOption->damage * 1.5f, block);	}
		else		{ damage = getStatsComp()->TakeDamage(damageOption->damage, block);			}

		// Damage Text
		CreateDamageText(damage, damageCauser->isLookingRight(), crit);

		AddImpulse(impulse, HIT_TIME);

		// Change State
		if (!block)
		{
			NewState(EBaseStates::Hit, "Hit", 0, false, true);
			EndStateDeferred(0.4f);
			

			Dead = getStatsComp()->getHealth() < 0.f;
			
			if (Dead)
			{
				GameInsRef->UnitKilled(damageCauser, this);
			}

			// Falling
			if (damageOption->fall || Dead)
			{
				FallDeferred(HIT_TIME);
				SetRotation(impulse.X < 0.f, false);
				AddImpulse(impulse + FVector2D(0.f, 100.f), HIT_TIME);
			}
		}
	}
	
	
	
	
	void AUnitBase::FallDeferred(float time)
	{
		SET_TIMER(fallTimer, this, &AUnitBase::Fall, time);
	}
	
	void AUnitBase::Fall()
	{
		if (State == EBaseStates::Hit)
		{
			NewState(EBaseStates::Fall, "FallUp", 0, false, false);
			EndStateDeferred(3.f);
		}
	}

	void AUnitBase::SetBlockingAttack(EBlockType type, float start, float end)
	{
		nBlockT = type;

		if (start > 0.f) { SET_TIMER(blockTimer, this, &AUnitBase::SetBlockType, start); }
		else { SetBlockType(); }

		if (start + end > 0.f) { SET_TIMER(blockEndTimer, this, &AUnitBase::DisableBlocking, start + end); }
		else { DisableBlocking(); }
	}

	void AUnitBase::SetBlockType()
	{
		BlockAttackType = nBlockT;
	}
	void AUnitBase::DisableBlocking()
	{
		BlockAttackType = EBlockType::None;
	}

	void AUnitBase::StandUp()
	{
		if (Dead)
			return;

		if (GetCharacterMovement()->IsMovingOnGround() 
			&& FMath::IsNearlyZero(GetVelocity().X + GetVelocity().Z))
		{
			NewState(EBaseStates::StandUp, "StandUp");
		}
		else
		{
			if (GetVelocity().Z > 20.f)
				NewState(EBaseStates::StandUp, "StandUpAir");
		}
	}
	void AUnitBase::CreateSpark(uint8 index, FVector2D scale, float rotation)
	{
		// Spawn Location
		FVector nLocation = GetActorLocation();
		nLocation.X += FMath::FRandRange(-15.f, 15.f);
		nLocation.Z += FMath::FRandRange(-15.f, 15.f);
		nLocation.Y += 1.f;

		// Spawn Transform
		FTransform nTransform
		{
			FRotator::ZeroRotator,
			nLocation,
			{scale.X, 1.f, scale.Y}
		};

		AHelper* nSpark = GetWorld()->SpawnActorDeferred<AHitSparkBase>(AHitSparkBase::StaticClass(), nTransform);
		if (nSpark)
		{
			// Init animation and rotation
			nSpark->Init(GameInsRef->getSpark(index), rotation);
			
			// Finish Spawning
			UGameplayStatics::FinishSpawningActor(nSpark, nTransform);

			// Attach to this actor
			//nSpark->AttachToActor(this,	FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true));
			//nSpark->SetActorLocation(nLocation);
		}
	}
	void AUnitBase::CreateDamageText(float damage, bool moveRight, bool crit)
	{
		if (!GameInsRef->canCreateDamageText(crit))
			return;

		// Location
		FTransform nTransform
		{
			FRotator::ZeroRotator,
			GetActorLocation(),
			FVector::OneVector
		};

		// Spawning
		AHelper* nText = GetWorld()->SpawnActorDeferred<AHelper>(GameInsRef->getDamageTextClass(), nTransform);
		if (nText)
		{
			nText->Init(damage, crit, moveRight);
			UGameplayStatics::FinishSpawningActor(nText, nTransform);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Creating damage text failed"));
		}
	}
// End Taking Damage //==========================------------------------------

// State Type //=================================------------------------------	
	void AUnitBase::NewState(uint8 state, FName anim, uint8 frame, bool ctrl, bool endState)
	{
		State = state;
		UPaperFlipbook* nAnim{ AnimData->FindRef(anim) };
		if (nAnim)
		{
			Control = ctrl;
			GetSprite()->SetFlipbook(nAnim);
			GetSprite()->SetLooping(!endState);
			GetSprite()->SetPlaybackPositionInFrames(frame, false);
			if (endState)
			{
				EndStateDeferred(getFrameTime(nAnim->GetNumFrames() - frame));
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

	void AUnitBase::NewState(uint8 state)
	{
		EndState();
		State = state;
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
		// �� ������ ������
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
				NewState(EBaseStates::Stand, "None");
			}
			else
			{
				NewState(EBaseStates::Jumping, "None");
			}
		}
	}
	
	void AUnitBase::AddAnimation(FName name, FString flipbookPath)
	{
		if(AnimData)
		{
			UPaperFlipbook* nAnim{ nullptr };
			nAnim = findObject(flipbookPath);
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
	
	void AUnitBase::SetAnim(UPaperFlipbook * anim, bool playFromStart)
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

// End State Type //=============================------------------------------	


//---------------------------------------------// Attack Danger Notification
	void AUnitBase::DangerN(float duration, EDangerType type)
	{
		DangerNoticeType = type;
		SET_TIMER(DamageNTimer, this, &AUnitBase::DangerNFinish, duration);
	}

	void AUnitBase::DangerNFinish()
	{
		DangerNoticeType = EDangerType::None;
	}