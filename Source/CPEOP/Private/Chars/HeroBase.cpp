// Fill out your copyright notice in the Description page of Project Settings.


#include "Chars/HeroBase.h"

#include "sys/MyPlayerController.h"
#include "sys/MyGameInstance.h"
#include "Paper2D/Classes/PaperFlipbookComponent.h"

#include "Components/SceneComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetMathLibrary.h"

// Camera Settings
#define ARM_LOCATION		FVector(80, 0, 0)
#define ARM_ROTATION		FRotator(-20, -90, 0)
#define ARM_LENGTH			600.f

#define CAM_INTERP			3.f
#define CAM_TARGET_DIST		350.f

// Movement Settings
#define MAX_WALK_SPEED		200.f
#define GRAVITY_SCALE		1.2f
#define MAX_ACCELERATION	4000.f

#define WALK_ERROR_T		20.f
#define JUMP_ERROR_T		50.f

// Dash Settings
#define DASH_VELOCITY		250.f
#define DASH_VELOCITY_Z		200.f
#define DASH_WAIT_TIME		0.2f

// Action Settings
#define SKILL_TIMER			0.2f
#define POW_CHARGE_TIMER	0.5f



FName AHeroBase::ArmCompName(TEXT("ArmComp"));
FName AHeroBase::CameraCompName(TEXT("CameraComp"));
FName AHeroBase::HeroStatsCompName(TEXT("HeroStatsComp"));


AHeroBase::AHeroBase()
{
	// Camera Scene Component Settings
	CameraSceneComp = CreateDefaultSubobject<USceneComponent>("SceneComp");
	if (CameraSceneComp)
	{
		CameraSceneComp->SetupAttachment(RootComponent);
		CameraSceneComp->SetRelativeLocation(ARM_LOCATION);
	}

	// Spring Arm Component Settings
	ArmComp = CreateOptionalDefaultSubobject<USpringArmComponent>(ArmCompName);
	if (ArmComp)
	{
		ArmComp->SetupAttachment(RootComponent);
		ArmComp->bDoCollisionTest = false;
		ArmComp->SetAbsolute(true, true, false);
		ArmComp->SetWorldRotation(ARM_ROTATION);
		ArmComp->TargetArmLength = ARM_LENGTH;
	}

	// Camera Component Settings
	CameraComp = CreateOptionalDefaultSubobject<UCameraComponent>(CameraCompName);
	if (CameraComp)
	{
		CameraComp->SetupAttachment(ArmComp);
		CameraComp->SetProjectionMode(ECameraProjectionMode::Orthographic);
		CameraComp->OrthoWidth = 450;
		CameraComp->OrthoFarClipPlane = 1500;
	}
	
	HeroStatsComp = CreateDefaultSubobject<UHeroStats>(HeroStatsCompName);

	// Movement Component Settings
	GetCharacterMovement()->MaxWalkSpeed = MAX_WALK_SPEED;
	GetCharacterMovement()->GravityScale = GRAVITY_SCALE;
	GetCharacterMovement()->Mass = 30;
	GetCharacterMovement()->MaxAcceleration = MAX_ACCELERATION;

	// Combo Keys
	resetKeys();

	// Helpers
	InitHelper("Teleport", "Blueprint/Objects/Dynamic/TeleportEff");

}

void AHeroBase::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<AMyPlayerController>(GetController());
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AHeroBase::OnCompHit);
}

void AHeroBase::Tick(float delta)
{
	Super::Tick(delta);
	UpdateCameraView(delta);
	UpdateAnim();
	TeleportTick(delta);
}

void AHeroBase::EndState()
{
	Super::EndState();
	resetKeys();
}

AMyPlayerController * AHeroBase::getController()
{
	if (PlayerController)
	{
		return PlayerController;
	}
	else
	{
		PlayerController = Cast<AMyPlayerController>(GetController());
	}
	return nullptr;
}

// Movement // Sprint // Dash //
	void AHeroBase::Sprint(FVector fwVector)
	{
		GetCharacterMovement()->MaxWalkSpeed = getHeroStatsComp()->getSprintSpeed();
		SET_TIMER(SprintPowReducingTimer, this, &AHeroBase::SprintPowReducing, cTime(0.05f), true);
		Sprinting = true;
	}

	void AHeroBase::StopSprinting()
	{
		GetCharacterMovement()->MaxWalkSpeed = getHeroStatsComp()->getWalkSpeed();
		PAUSE_TIMER(SprintPowReducingTimer);
		Sprinting = false;
	}

	void AHeroBase::SprintPowReducing()
	{
		getHeroStatsComp()->AddStamina(-0.01f);
	}

	void AHeroBase::Dash(FVector fwVector)
	{
		fwVector *= DASH_VELOCITY;
		fwVector.Z = DASH_VELOCITY_Z;
		DashVector = fwVector;

		if ((isComboTime() && GetCharacterMovement()->IsMovingOnGround()) || (checkState(EBaseStates::Blocking) && GetCharacterMovement()->IsMovingOnGround()))
		{
			DoDash();
		}
		else
		{
			addKey(EComboKey::CK_Dash);
		}
	}

	void AHeroBase::DoDash()
	{
		NewState(EBaseStates::Dash, "JumpLand");
		AddImpulse(DashVector, cTime(DASH_WAIT_TIME));
		EndStateDeferred(DASH_WAIT_TIME + 0.01f);
	}
// End

// Blocking Movement //==========================------------------------------

	void AHeroBase::Move()
	{
		if (Control)
		{
			float MovementScale = 1.f;
			if (LockUp)
			{
				if (MoveVector.Y < -0.7f) MovementScale = 0.f;
				else UnlockMovementY();
			}
			else if (LockDown)
			{
				if (MoveVector.Y > 0.7f) MovementScale = 0.f;
				else UnlockMovementY();
			}

			if (LockForward)
			{
				if (MoveVector.X > 0.7f) MovementScale = 0.f;
				else UnlockMovementX();
			}
			else if (LockBack)
			{
				if (MoveVector.X < -0.7f) MovementScale = 0.f;
				else UnlockMovementX();
			}

			AddMovementInput(MoveVector, MovementScale);
			if (MoveVector != FVector::ZeroVector)
			{
				SetRotation(isMovingRight());
			}
		}
	}

	void AHeroBase::UpdateAnim()
	{
		const FVector velocity = GetVelocity();
		UPaperFlipbook* anim{ nullptr };
		switch (getState())
		{
		case EBaseStates::Stand:
		{
			if (!(GetCharacterMovement()->IsMovingOnGround()))
			{
				NewState(EBaseStates::Jumping, "None");
				return;
			}

			if (!FMath::IsNearlyZero(velocity.X, WALK_ERROR_T) || !FMath::IsNearlyZero(velocity.Y, WALK_ERROR_T))
			{
				anim = AnimData->FindRef("Walk");
			}
			else
			{
				anim = AnimData->FindRef("Stand");
			}
			break;
		}
		case EBaseStates::Jumping:
		{
			if (velocity.Z > JUMP_ERROR_T)
			{
				anim = AnimData->FindRef("JumpUp");
			}
			else if (velocity.Z < -JUMP_ERROR_T)
			{
				anim = AnimData->FindRef("JumpDown");
			}
			else
			{
				anim = AnimData->FindRef("JumpHold");
			}
			break;
		}
		case EBaseStates::Blocking:
		{
			if (GetCharacterMovement()->IsMovingOnGround())
			{
				anim = AnimData->FindRef("Block");
			}
			else
			{
				anim = AnimData->FindRef("BlockAir");
			}
			break;
		}
		case EBaseStates::Fall:
		{
			if (GetCharacterMovement()->IsMovingOnGround())
			{
				anim = AnimData->FindRef("FallHold");
			}
			else if (velocity.Z > 0.f)	{ anim = AnimData->FindRef("FallUp");	}
			else						{ anim = AnimData->FindRef("FallDown"); }
			break;
		}

		} // End Switch

		if (anim)
		{
			GetSprite()->SetFlipbook(anim);
		}

	} // End Function

	void AHeroBase::OnCompHit(
		UPrimitiveComponent*	HitComponent,
		AActor*					OtherActor,
		UPrimitiveComponent*	OtherComp,
		FVector					NormalImpulse,
		const FHitResult&		Hit)
	{
		if (OtherComp->ComponentHasTag("Wall"))
		{
			LockMovement(Hit.ImpactPoint);
		}
	}

	void AHeroBase::LockMovement(FVector Point)
	{
		Point = GetActorLocation() - Point;

		if		(Point.Y >	5.f)	{ LockUp		= true;	}
		else if (Point.Y < -5.f)	{ LockDown		= true; }
		else if (Point.X >	5.f)	{ LockBack		= true; }
		else if (Point.X < -5.f)	{ LockForward	= true; }
	}

	void AHeroBase::UnlockMovementX()
	{
		LockForward = false;
		LockBack	= false;
	}

	void AHeroBase::UnlockMovementY()
	{
		LockUp		= false;
		LockDown	= false;
	}
// End Blocking Movement //======================------------------------------

// Camera Behaviour //===========================------------------------------
	void AHeroBase::UpdateCameraView(float delta)
	{
		switch (CameraMode)
		{
		case ECameraMode::Free: 
		{
			const FVector ArmLoc = ArmComp->GetComponentLocation();
			const FVector CamPointLoc = CameraSceneComp->GetComponentLocation();
			
			// New Location
			FVector nLoc = FVector::ZeroVector;
			nLoc.X = FMath::Clamp(CamPointLoc.X, CameraXClampA, CameraXClampB);
			nLoc.Y = CamPointLoc.Y;
			nLoc.Z = CamPointLoc.Z;

			ArmComp->SetWorldLocation(FMath::VInterpTo(ArmLoc, nLoc, delta, CAM_INTERP));

			if (CameraTargetActor)
			{
				if (FVector::Distance(GetActorLocation(), CameraTargetActor->GetActorLocation()) < CAM_TARGET_DIST)
				{
					CameraMode = ECameraMode::Target;
				}
			}
			return;
		}
		case ECameraMode::Action:
		{
			const FVector ArmLoc = ArmComp->GetComponentLocation();

			ArmComp->SetWorldLocation(FMath::VInterpTo(ArmLoc, CameraView, delta, CAM_INTERP));
			return;
		}
		case ECameraMode::Target:
		{
			if (CameraTargetActor)
			{
				const FVector myLoc = GetActorLocation();
				const FVector targetLoc = CameraTargetActor->GetActorLocation();

				FVector nLoc
				{
					FMath::Lerp(myLoc.X, targetLoc.X, 0.5f), 
					FMath::Lerp(myLoc.Y, targetLoc.Y, 0.5f),
					FMath::Lerp(myLoc.Z, targetLoc.Z, 0.5f)
				};

				ArmComp->SetWorldLocation(FMath::VInterpTo(ArmComp->GetComponentLocation(), nLoc, delta, CAM_INTERP));

				if (FVector::Distance(myLoc, targetLoc) > CAM_TARGET_DIST)
				{
					CameraMode = ECameraMode::Free;
				}
			}
			else
			{
				CameraMode = ECameraMode::Free;
			}
		}
		} // End Switch
	}

	void AHeroBase::SetCameraViewF(float ClampA, float ClampB)
	{
		if (ClampA != CameraXClampA || ClampB != CameraXClampB)
		{
			CameraXClampA = ClampA;
			CameraXClampB = ClampB;

			const FVector CameraPointLoc = CameraSceneComp->GetComponentLocation();
			ArmComp->SetWorldLocation(
				FVector(
					FMath::Clamp(CameraPointLoc.X, CameraXClampA, CameraXClampB),
					0, CameraPointLoc.Z)
			);
		}
		CameraMode = ECameraMode::Free;
	}

	void AHeroBase::SetCameraViewA(FVector CameraLocation)
	{
		CameraView = CameraLocation;
		CameraMode = ECameraMode::Action;
		
	}

	void AHeroBase::SetCameraTarget(APawn * target)
	{
		if (target == nullptr || target == this)
		{
			CameraMode = ECameraMode::Free;
		}
		else
		{
			CameraTargetActor = target;
			CameraMode = ECameraMode::Target;
		}
	}
// End Camera Behaviour //=======================------------------------------

// Actions //====================================------------------------------
	void AHeroBase::Attack()
	{
		
	}
	
	void AHeroBase::AttackHold()
	{
		SET_TIMER(skillEnTimer, this, &AHeroBase::SkillEnable, cTime(SKILL_TIMER));
		SET_TIMER(PowChargeTimer, this, &AHeroBase::PowCharge, cTime(POW_CHARGE_TIMER));
	}
	
	void AHeroBase::AttackBack()
	{
	}
	
	void AHeroBase::AttackForward()
	{
	}
	
	void AHeroBase::Block()
	{
		if (getState() == EBaseStates::Stand || getState() == EBaseStates::Jumping)
		{
			NewState(EBaseStates::Blocking, "Block", 0, false, false);
			if (!FMath::IsNearlyZero(MoveVector.X))
			{
				SetRotation(isMovingRight());
			}
			PAUSE_TIMER(BlockTimer);
			PowChargeEnd();
		}
		else
		{
			SET_TIMER(BlockTimer, this, &AHeroBase::Block, 0.1f, true);
		}
	}
	
	void AHeroBase::BlockStop()
	{
		PAUSE_TIMER(BlockTimer);
		if (getState() == EBaseStates::Blocking)
		{
			EndStateDeferred(cTime(0.1f));
		}
	}

	void AHeroBase::PowCharge()
	{
		if (getState() == EBaseStates::Stand)
		{
			NewState(EBaseStates::PowCharge, "PowChargeStart", 0, false, false);
			SET_TIMER(PowChargeLoopTimer, this, &AHeroBase::PowCharge, cTime(0.35f));
		}
		else if (getState() == EBaseStates::PowCharge)
		{
			NewState(EBaseStates::PowChargeLoop, "PowChargeLoop", 0, false, false);
			SET_TIMER(PowChargeLoopTimer, this, &AHeroBase::PowChargeLoop, cTime(0.05f));
		}
	}
	
	void AHeroBase::PowChargeLoop()
	{
		if (getState() == EBaseStates::PowChargeLoop)
		{
			SET_TIMER(PowChargeLoopTimer, this, &AHeroBase::PowChargeLoop, cTime(0.05f));
			// Add Power
			GET_STATS->AddStamina(0.02f + getHeroStatsComp()->getStamina() / 100.f);
		}

	}
	
	void AHeroBase::PowChargeEnd()
	{
		PAUSE_TIMER(PowChargeTimer);
		PAUSE_TIMER(PowChargeLoopTimer);

		if (getState() == EBaseStates::PowChargeLoop || getState() == EBaseStates::PowCharge)
		{
			NewState(EBaseStates::PowChargeEnd, "PowChargeEnd");
		}
	}

	void AHeroBase::SkillEnable()
	{
		Skill = true;
		SET_TIMER(skillDisTimer, this, &AHeroBase::SkillDisable, cTime(1.f));
		if (getController())
			getController()->HeroSkillActivated();
	}

	void AHeroBase::SkillDisable()
	{
		Skill = false;
		PAUSE_TIMER(skillDisTimer);
		if (getController())
			getController()->HeroSkillDeactivated();
	}

	void AHeroBase::SkillCanceled()
	{
		PAUSE_TIMER(skillEnTimer);
	}

// End Actions //================================------------------------------

// Actions Combination //========================------------------------------
	void AHeroBase::Combo(float time)
	{
		SET_TIMER(ComboTimer, this, &AHeroBase::ComboI, time, false);
		ComboSuccess = false;
	}

	void AHeroBase::ComboI()
	{
		ComboSuccess = true;


	}

	void AHeroBase::addKey(EComboKey key)
	{
		ComboKeys.Add(key);
	}
	
	void AHeroBase::resetKeys()
	{
		KeyIndex = 0;
		ComboKeys.Empty();
	}
	
	EComboKey AHeroBase::getNextKey()
	{
		++KeyIndex;
		if (ComboKeys.Num() > KeyIndex) { return ComboKeys[KeyIndex];}
		else							{ return EComboKey::CK_None; }
	}
// End Actions Combination //====================------------------------------

//---------------------------------------------// Teleport
	void AHeroBase::Teleport()
	{
		if (!getHeroStatsComp()->checkStamina(1.F / getHeroStatsComp()->getTeleportCost(), false))
			return;

		if (MoveVector == FVector::ZeroVector || checkState(EBaseStates::Fall) || checkState(EBaseStates::Teleport) || !isAlive())
			return;

		if (BlockAttackType != EBlockType::None && !isComboTime())
			return;

		tp_initialLocation = GetActorLocation();
		tp_Vector = MoveVector;
		tp_DistPassed = 0.f;
		tp_MaxDist = 100.f;

		NewState(EBaseStates::Teleport);
		SetAnim(nullptr, true);
	
		FRotator tp_HelperRot = tp_Vector.ToOrientationRotator();
		FVector tp_HelperScale = { 1.f, 1.f, 1.f };
		if (FMath::IsNearlyEqual(tp_HelperRot.Yaw, 90.f, 10.f))
		{
			tp_HelperRot.Roll	= 90.f;
			tp_HelperRot.Yaw	= 90.f;
			tp_HelperScale.X	= 2.f;
		}
		else if (FMath::IsNearlyEqual(tp_HelperRot.Yaw, -90.f, 10.f))
		{
			tp_HelperRot.Roll	= -90.f;
			tp_HelperRot.Yaw	= -90.f;
			tp_HelperScale.X	= 2.f;
		}
		SpawnHelper("Teleport", 0.f, tp_HelperRot, tp_HelperScale);
	}
	void AHeroBase::Teleport(FVector nLocation)
	{
		if (!getHeroStatsComp()->checkStamina(1.F / getHeroStatsComp()->getTeleportCost(), false))
			return;

		if (checkState(EBaseStates::Fall) || checkState(EBaseStates::Teleport) || !isAlive())
			return;

		tp_initialLocation = GetActorLocation();
		nLocation.Z = tp_initialLocation.Z;
		FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(tp_initialLocation, nLocation);
		tp_Vector = UKismetMathLibrary::GetForwardVector(lookRotation);
		tp_DistPassed = 0.f;
		tp_MaxDist = FVector::Dist(tp_initialLocation, nLocation);

		NewState(EBaseStates::Teleport);
		SetAnim(nullptr, true);
		
		FRotator& tp_HelperRot = lookRotation;
		FVector tp_HelperScale = { 1.f, 1.f, 1.f };

		if (FMath::IsNearlyEqual(tp_HelperRot.Yaw, 90.f, 10.f))
		{
			tp_HelperRot.Roll	= 90.f;
			tp_HelperRot.Yaw	= 90.f;
			tp_HelperScale.X	= 2.f;
		}
		else if (FMath::IsNearlyEqual(tp_HelperRot.Yaw, -90.f, 10.f))
		{
			tp_HelperRot.Roll	= -90.f;
			tp_HelperRot.Yaw	= -90.f;
			tp_HelperScale.X	= 2.f;
		}
		SpawnHelper("Teleport", 0.f, tp_HelperRot, tp_HelperScale);
	}

	inline void AHeroBase::TeleportTick(float delta)
	{
		if (getState() == EBaseStates::Teleport)
		{
			float speed{ 1000 * delta };
			tp_DistPassed += speed;
			SetActorLocation(GetActorLocation() + tp_Vector * speed, true);
			if (tp_DistPassed >= tp_MaxDist)
			{
				GetCharacterMovement()->Velocity = tp_Vector * 200.f;
				EndState();

				// Minus stamina
				float dist = FVector::Dist(tp_initialLocation, GetActorLocation());
				float minusStamina = FMath::Min(1.2f, dist / 100.f) / getHeroStatsComp()->getTeleportCost();
				GET_STATS->AddStamina(-(minusStamina));
			}
		}
	}

// Transformation
	void AHeroBase::ChangeForm(FName formName)
	{
		getHeroStatsComp()->SetForm(formName);
		GetCharacterMovement()->MaxWalkSpeed = getHeroStatsComp()->getWalkSpeed();
	}
	void AHeroBase::InitForm(FName formName, FVector stats)
	{
		getHeroStatsComp()->AddForms(formName, stats);
	}
// End