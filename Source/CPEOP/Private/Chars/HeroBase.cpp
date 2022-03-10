// Fill out your copyright notice in the Description page of Project Settings.

#include "Chars/HeroBase.h"
#include "sys/MyGameInstance.h"

// Components
#include "Chars/Components/ShadowComponent.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Function Library
#include "Kismet/KismetMathLibrary.h"
#include "Sys/MyFunctionLibrary.h"
#include "UObject/ConstructorHelpers.h"

// Camera Settings
#define ARM_LOCATION	 FVector(80, 0, 0)
#define ARM_ROTATION	 FRotator(-20, -90, 0)
#define ARM_LENGTH		 600.f

#define CAM_INTERP		 3.f

// Movement Settings
#define MAX_WALK_SPEED	 200.f
#define GRAVITY_SCALE	 1.2f
#define MAX_ACCELERATION 4000.f

#define WALK_ERROR_T	 20.f
#define JUMP_ERROR_T	 50.f

// Dash Settings
#define DASH_VELOCITY	 250.f
#define DASH_VELOCITY_Z	 300.f

// Action Settings
#define SKILL_TIMER		 0.2f
#define POW_CHARGE_TIMER 0.5f

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
		CameraComp->OrthoWidth		  = 450;
		CameraComp->OrthoFarClipPlane = 1500;
	}

	HeroStatsComp = CreateDefaultSubobject<UHeroStats>(HeroStatsCompName);

	// Movement Component Settings
	GetCharacterMovement()->MaxWalkSpeed	= MAX_WALK_SPEED;
	GetCharacterMovement()->GravityScale	= GRAVITY_SCALE;
	GetCharacterMovement()->Mass			= 30;
	GetCharacterMovement()->MaxAcceleration = MAX_ACCELERATION;
	GetCharacterMovement()->JumpZVelocity	= 500.f;

	// Helpers
	InitHelper("Teleport", "Blueprint/Objects/Dynamic/TeleportEff");
}

void AHeroBase::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AHeroBase::OnCompHit);
}

void AHeroBase::Tick(float delta)
{
	Super::Tick(delta);

	UpdateCameraView(delta);
	UpdateAnim();
	TeleportTick(delta);
	CurveTimeline.TickTimeline(delta);
}

void AHeroBase::SetHeroName(FName NewName)
{
	HeroName = NewName;
}

// Timeline
UCurveFloat* AHeroBase::FindCurveFloat(FString path)
{
	FString objectName = UMyFunctionLibrary::FindObjectName(path);
	FString fullPath   = "CurveFloat'/Game/" + path + "." + objectName + "'";

	ConstructorHelpers::FObjectFinder<UCurveFloat> nCurve((TEXT("%s"), *fullPath));
	if (nCurve.Succeeded())
	{
		return nCurve.Object;
	}
	return nullptr;
}

void AHeroBase::PlayTimeline(UObject* targetObject, UCurveFloat* curve, FName functionName, bool looping)
{
	if (curve)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(targetObject, functionName);
		CurveTimeline.AddInterpFloat(curve, TimelineProgress);
		CurveTimeline.SetLooping(looping);
		CurveTimeline.PlayFromStart();
	}
}

void AHeroBase::StopTimeline()
{
	CurveTimeline.Stop();
}

//
void AHeroBase::EndState()
{
	Super::EndState();
	ResetKeys();
	SkillDisable();
}

// Movement // Sprint // Dash //
void AHeroBase::Sprint(FVector fwVector)
{
	GetCharacterMovement()->MaxWalkSpeed = GetHeroStats()->getSprintSpeed();
	SET_TIMER(SprintPowReducingTimer, this, &AHeroBase::SprintPowReducing, cTime(0.05f), true);
	Sprinting = true;
}

void AHeroBase::StopSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed = GetHeroStats()->getWalkSpeed();
	PAUSE_TIMER(SprintPowReducingTimer);
	Sprinting = false;
}

void AHeroBase::SprintPowReducing()
{
	GetHeroStats()->AddStamina(-0.01f);
}

void AHeroBase::Dash(FVector fwVector)
{
	fwVector *= DASH_VELOCITY;
	fwVector.Z = DASH_VELOCITY_Z;
	DashVector = fwVector;

	if (GetCharacterMovement()->IsMovingOnGround() && (isComboTime() || CheckState(EBaseStates::Blocking)) ||
		CheckState(EBaseStates::JumpLand))
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
	FState nState;
	nState.State		  = EBaseStates::Dash;
	nState.Animation	  = "JumpStart";
	nState.AnimationFrame = (CheckState(EBaseStates::JumpLand)) ? 1 : 0;
	nState.Rotate		  = false;
	nState.EndState		  = false;
	NewState(nState);

	float DashTime = AnimElemTime(GetAnim("JumpStart")->GetNumFrames() - nState.AnimationFrame);

	AddImpulse(DashVector, DashTime);
	EndStateDeferred(DashTime + cTime(0.01f));
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
			if (MoveVector.Y < -0.7f)
				MovementScale = 0.f;
			else
				UnlockMovementY();
		}
		else if (LockDown)
		{
			if (MoveVector.Y > 0.7f)
				MovementScale = 0.f;
			else
				UnlockMovementY();
		}

		if (LockForward)
		{
			if (MoveVector.X > 0.7f)
				MovementScale = 0.f;
			else
				UnlockMovementX();
		}
		else if (LockBack)
		{
			if (MoveVector.X < -0.7f)
				MovementScale = 0.f;
			else
				UnlockMovementX();
		}

		AddMovementInput(MoveVector, MovementScale);
		if (MoveVector != FVector::ZeroVector)
		{
			SetRotation(IsMovingRight());
		}
	}
}

void AHeroBase::UpdateAnim()
{
	const FVector	velocity = GetVelocity();
	UPaperFlipbook* anim{nullptr};
	switch (GetState())
	{
	case EBaseStates::Stand:
	{
		if (! (GetCharacterMovement()->IsMovingOnGround()))
		{
			FState nState;
			nState.State = EBaseStates::Jumping;
			NewState(nState);
			return;
		}

		if (! FMath::IsNearlyZero(velocity.X, WALK_ERROR_T) || ! FMath::IsNearlyZero(velocity.Y, WALK_ERROR_T))
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
		if (FMath::IsNearlyZero(GetUnitVelocity().Z, 50.f))
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
		else if (velocity.Z > 0.f)
		{
			anim = AnimData->FindRef("FallUp");
		}
		else
		{
			anim = AnimData->FindRef("FallDown");
		}
		break;
	}

	} // End Switch

	if (anim)
	{
		GetSprite()->SetFlipbook(anim);
	}

} // End Function

void AHeroBase::OnCompHit(
	UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherComp->ComponentHasTag("Wall"))
	{
		LockMovement(Hit.ImpactPoint);
	}
}

void AHeroBase::LockMovement(FVector Point)
{
	Point = GetActorLocation() - Point;

	if (Point.Y > 5.f)
	{
		LockUp = true;
	}
	else if (Point.Y < -5.f)
	{
		LockDown = true;
	}
	else if (Point.X > 5.f)
	{
		LockBack = true;
	}
	else if (Point.X < -5.f)
	{
		LockForward = true;
	}
}

void AHeroBase::UnlockMovementX()
{
	LockForward = false;
	LockBack	= false;
}

void AHeroBase::UnlockMovementY()
{
	LockUp	 = false;
	LockDown = false;
}
// End Blocking Movement //======================------------------------------

// Camera Behaviour //===========================------------------------------
FVector AHeroBase::GetCameraLocation()
{
	FVector CamPointLoc = CameraSceneComp->GetComponentLocation();
	FVector nLoc		= FVector::ZeroVector;
	nLoc.X				= FMath::Clamp(CamPointLoc.X, CameraXClampA, CameraXClampB);
	nLoc.Y				= FMath::Clamp(CamPointLoc.Y, CameraYClampA, CameraYClampB);
	nLoc.Z				= CamPointLoc.Z;
	return nLoc;
}

void AHeroBase::UpdateCameraView(float delta)
{
	switch (CameraMode)
	{
	case ECameraMode::Free:
	{
		const FVector ArmLoc	  = ArmComp->GetComponentLocation();
		const FVector CamPointLoc = CameraSceneComp->GetComponentLocation();

		// New Location
		FVector nLoc = FVector::ZeroVector;
		nLoc.X		 = FMath::Clamp(CamPointLoc.X, CameraXClampA, CameraXClampB);
		nLoc.Y		 = FMath::Clamp(CamPointLoc.Y, CameraYClampA, CameraYClampB);
		nLoc.Z		 = CamPointLoc.Z;

		ArmComp->SetWorldLocation(FMath::VInterpTo(ArmLoc, nLoc, delta, CAM_INTERP));

		if (CameraTargetActor)
		{
			if (FVector::Distance(GetActorLocation(), CameraTargetActor->GetActorLocation()) < CameraTargetDist &&
				CameraTargetActor->GetActorLocation().Z < CameraTargetDist / 2.f)
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
		if (IsValid(CameraTargetActor))
		{
			const FVector myLoc		= GetActorLocation();
			const FVector targetLoc = CameraTargetActor->GetActorLocation();

			if (FVector::Distance(myLoc, targetLoc) > CameraTargetDist || targetLoc.Z > CameraTargetDist / 2.f)
			{
				CameraMode = ECameraMode::Free;
				return;
			}

			FVector nLoc{FMath::Clamp(FMath::Lerp(myLoc.X, targetLoc.X, 0.5f), CameraXClampA, CameraXClampB),
				FMath::Clamp(FMath::Lerp(myLoc.Y, targetLoc.Y, 0.5f), CameraYClampA, CameraYClampB),
				FMath::Lerp(myLoc.Z, targetLoc.Z, 0.5f)};

			ArmComp->SetWorldLocation(FMath::VInterpTo(ArmComp->GetComponentLocation(), nLoc, delta, CAM_INTERP));
		}
		else
		{
			CameraMode		  = ECameraMode::Free;
			CameraTargetActor = nullptr;
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
		ArmComp->SetWorldLocation(FVector(FMath::Clamp(CameraPointLoc.X, CameraXClampA, CameraXClampB), 0, CameraPointLoc.Z));
	}
	CameraMode = ECameraMode::Free;
}

void AHeroBase::SetCameraViewA(FVector CameraLocation, float Duration, ECameraMode NextCamMode)
{
	CameraLastMode = NextCamMode;
	CameraView	   = CameraLocation;
	CameraMode	   = ECameraMode::Action;

	SET_TIMER(CamActionTimer, this, &AHeroBase::DisableCameraViewA, FMath::Max(Duration, 0.2f));
}

void AHeroBase::DisableCameraViewA()
{
	CameraMode = CameraLastMode;
}

void AHeroBase::SetCameraTarget(AUnitBase* target, float dist)
{
	if (target == nullptr || target == this)
	{
		CameraTargetActor = nullptr;
	}
	else
	{
		CameraTargetActor = target;
		CameraTargetDist  = dist;
		CameraMode		  = ECameraMode::Target;
	}
}
// End Camera Behaviour //=======================------------------------------

// Actions //====================================------------------------------
void AHeroBase::Attack() {}

void AHeroBase::AttackHold()
{
	SET_TIMER(skillEnTimer, this, &AHeroBase::SkillEnable, cTime(SKILL_TIMER));
	SET_TIMER(PowChargeTimer, this, &AHeroBase::PowCharge, cTime(POW_CHARGE_TIMER));
}

void AHeroBase::AttackBack() {}

void AHeroBase::AttackForward() {}

void AHeroBase::Block()
{
	if (CheckState(EBaseStates::Stand) || CheckState(EBaseStates::Jumping))
	{
		FState nState;
		nState.State	 = EBaseStates::Blocking;
		nState.Animation = "Block";
		nState.EndState	 = false;
		NewState(nState);

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
	if (GetState() == EBaseStates::Blocking)
	{
		EndStateDeferred(cTime(0.1f));
	}
}

void AHeroBase::PowCharge()
{
	if (CheckState(EBaseStates::Stand))
	{
		FState nState;
		nState.State	 = EBaseStates::PowCharge;
		nState.Animation = "PowChargeStart";
		nState.Rotate	 = false;
		nState.EndState	 = false;
		NewState(nState);

		SET_TIMER(PowChargeLoopTimer, this, &AHeroBase::PowCharge, AnimElemTime(GetAnim(nState.Animation)->GetNumFrames()));
	}
	else if (CheckState(EBaseStates::PowCharge))
	{
		FState nState;
		nState.State	 = EBaseStates::PowChargeLoop;
		nState.Animation = "PowChargeLoop";
		nState.Rotate	 = false;
		nState.EndState	 = false;
		NewState(nState);

		SET_TIMER(PowChargeLoopTimer, this, &AHeroBase::PowChargeLoop, cTime(0.05f));
	}
}

void AHeroBase::PowChargeLoop()
{
	if (CheckState(EBaseStates::PowChargeLoop))
	{
		SET_TIMER(PowChargeLoopTimer, this, &AHeroBase::PowChargeLoop, cTime(0.05f));
		// Add Power
		GET_STATS->AddStamina(0.02f + GetHeroStats()->getStamina() / 100.f);
	}
}

void AHeroBase::PowChargeEnd()
{
	PAUSE_TIMER(PowChargeTimer);
	PAUSE_TIMER(PowChargeLoopTimer);

	if (CheckState(EBaseStates::PowChargeLoop) || CheckState(EBaseStates::PowCharge))
	{
		FState nState;
		nState.State	 = EBaseStates::PowChargeEnd;
		nState.Animation = "PowChargeEnd";
		nState.Rotate	 = false;
		NewState(nState);
	}
}

void AHeroBase::SkillEnable()
{
	Skill = true;
	SET_TIMER(skillDisTimer, this, &AHeroBase::SkillDisable, cTime(1.f));
	HeroSkillActivated();
}

void AHeroBase::SkillDisable()
{
	Skill = false;
	PAUSE_TIMER(skillDisTimer);
	HeroSkillDeactivated();
}

void AHeroBase::SkillCanceled()
{
	PAUSE_TIMER(skillEnTimer);
}

// End Actions //================================------------------------------

// Actions Combination //========================------------------------------
void AHeroBase::Combo(float time)
{
	if (time > 0.f)
	{
		SET_TIMER(ComboTimer, this, &AHeroBase::ComboI, time, false);
		ComboSuccess = false;
	}
}

void AHeroBase::ComboI()
{
	ComboSuccess = true;
}

void AHeroBase::addKey(EComboKey key)
{
	ComboKeys.Add(key);
}

void AHeroBase::ResetKeys()
{
	PAUSE_TIMER(ComboTimer);
	KeyIndex = 0;
	ComboKeys.Empty();
}

EComboKey AHeroBase::getNextKey()
{
	++KeyIndex;
	if (ComboKeys.Num() > KeyIndex)
	{
		return ComboKeys[KeyIndex];
	}
	else
	{
		return EComboKey::CK_None;
	}
}
// End Actions Combination //====================------------------------------

//---------------------------------------------// Teleport
bool AHeroBase::Teleport()
{
	if (IsImmortal())
		return false;

	if (! GetHeroStats()->checkStamina(1.f / GetHeroStats()->getTeleportCost(), false))
	{
		NotEnoughStamina();
		return false;
	}

	if (MoveVector == FVector::ZeroVector || CheckState(EBaseStates::Fall) || CheckState(EBaseStates::Teleport) || IsDead())
		return false;

	if (BlockAttackType != EBlockType::None && ! isComboTime())
		return false;

	tp_initialLocation = GetActorLocation();
	tp_Vector		   = MoveVector;
	tp_DistPassed	   = 0.f;
	tp_MaxDist		   = 100.f;

	NewState(EBaseStates::Teleport);
	SetAnim(nullptr, true);

	FRotator tp_HelperRot	= tp_Vector.ToOrientationRotator();
	FVector	 tp_HelperScale = {1.f, 1.f, 1.f};
	if (FMath::IsNearlyEqual(tp_HelperRot.Yaw, 90.f, 10.f))
	{
		tp_HelperRot.Roll = 90.f;
		tp_HelperRot.Yaw  = 90.f;
		tp_HelperScale.X  = 2.f;
	}
	else if (FMath::IsNearlyEqual(tp_HelperRot.Yaw, -90.f, 10.f))
	{
		tp_HelperRot.Roll = -90.f;
		tp_HelperRot.Yaw  = -90.f;
		tp_HelperScale.X  = 2.f;
	}
	SpawnHelperDeferred("Teleport", 0.f, tp_HelperRot, tp_HelperScale);
	getShadow()->HideShadow();
	return true;
}
void AHeroBase::Teleport(FVector nLocation)
{
	if (IsImmortal())
		return;

	if (! GetHeroStats()->checkStamina(1.f / GetHeroStats()->getTeleportCost(), false))
		return;

	if (CheckState(EBaseStates::Fall) || CheckState(EBaseStates::Teleport) || IsDead())
		return;

	if (BlockAttackType != EBlockType::None && ! isComboTime())
		return;

	tp_initialLocation	  = GetActorLocation();
	nLocation.Z			  = tp_initialLocation.Z;
	FRotator lookRotation = UKismetMathLibrary::FindLookAtRotation(tp_initialLocation, nLocation);
	tp_Vector			  = UKismetMathLibrary::GetForwardVector(lookRotation);
	tp_DistPassed		  = 0.f;
	tp_MaxDist			  = FVector::Dist(tp_initialLocation, nLocation);

	NewState(EBaseStates::Teleport);
	SetAnim(nullptr, true);

	FRotator& tp_HelperRot	 = lookRotation;
	FVector	  tp_HelperScale = {1.f, 1.f, 1.f};

	if (FMath::IsNearlyEqual(tp_HelperRot.Yaw, 90.f, 10.f))
	{
		tp_HelperRot.Roll = 90.f;
		tp_HelperRot.Yaw  = 90.f;
		tp_HelperScale.X  = 2.f;
	}
	else if (FMath::IsNearlyEqual(tp_HelperRot.Yaw, -90.f, 10.f))
	{
		tp_HelperRot.Roll = -90.f;
		tp_HelperRot.Yaw  = -90.f;
		tp_HelperScale.X  = 2.f;
	}
	SpawnHelperDeferred("Teleport", 0.f, tp_HelperRot, tp_HelperScale);
	getShadow()->HideShadow();
}

void AHeroBase::TeleportTick(float delta)
{
	if (CheckState(EBaseStates::Teleport))
	{
		float speed{1000 * delta};
		tp_DistPassed += speed;
		SetActorLocation(GetActorLocation() + tp_Vector * speed, true);
		if (tp_DistPassed >= tp_MaxDist)
		{
			EndState();
			GetCharacterMovement()->Velocity = tp_Vector * 300.f;

			// Minus stamina
			float dist		   = FVector::Dist(tp_initialLocation, GetActorLocation());
			float minusStamina = FMath::Min(1.2f, dist / 100.f) / GetHeroStats()->getTeleportCost();
			GET_STATS->AddStamina(-(minusStamina));
			getShadow()->ShowShadow();
		}
	}
}

// Transformation
void AHeroBase::ChangeForm(FName formName)
{
	GetHeroStats()->SetForm(formName);
	GetCharacterMovement()->MaxWalkSpeed = GetHeroStats()->getWalkSpeed();
}
void AHeroBase::InitForm(FName formName, FVector stats)
{
	GetHeroStats()->AddForms(formName, stats);
}
// End

//---------------------------------------------// Hero Inputs
void AHeroBase::BtnSetMovement_Implementation(FVector Value)
{
	SetMoveVector(Value);
}

void AHeroBase::BtnAction_Implementation(EInputActionType action, bool btnReleased)
{
	switch (action)
	{
	case EInputActionType::A_Center:
	{
		if (btnReleased)
		{
			Attack();
			addKey(EComboKey::CK_Attack);
		}
		else
		{
			AttackHold();
		}
		break;
	}
	case EInputActionType::A_Up:
	{
		if (btnReleased)
		{
			EventJump();
			addKey(EComboKey::CK_Jump);
		}
		break;
	}
	case EInputActionType::A_Down:
	{
		if (btnReleased)
		{
			BlockStop();
			AttackDown();
			addKey(EComboKey::CK_Block);
		}
		break;
	}
	case EInputActionType::A_Forward:
	{
		if (btnReleased)
		{
			if (IsLookingRight())
			{
				AttackForward();
				addKey(EComboKey::CK_AForward);
			}
			else
			{
				AttackBack();
				addKey(EComboKey::CK_ABackward);
			}
		}
		break;
	}
	case EInputActionType::A_Backward:
	{
		if (btnReleased)
		{
			if (IsLookingRight())
			{
				AttackBack();
				addKey(EComboKey::CK_ABackward);
			}
			else
			{
				AttackForward();
				addKey(EComboKey::CK_AForward);
			}
		}
		break;
	}

	case EInputActionType::A_BlockStart:
	{
		if (! IsSkillActive())
		{
			Block();
			SkillCanceled();
		}
		break;
	}
	case EInputActionType::A_BlockEnd:
	{
		BlockStop();
		break;
	}
	} // End switch

	if (btnReleased)
	{
		PowChargeEnd();
		SkillCanceled();
	}
}

void AHeroBase::BtnDash_Implementation(FVector forwardVector, bool Released)
{
	if (Released)
	{
		// Stop Sprinting
		if (isSprinting())
		{
			StopSprinting();
			return;
		}

		// Set Dash Forward Vector
		if (BtnDashVector.IsNearlyZero())
		{
			BtnDashVector = forwardVector;
			SET_TIMER(BtnDashTimer, this, &AHeroBase::BtnDashReset, cTime(0.3));
			return;
		}

		if (FVector::PointsAreNear(BtnDashVector, forwardVector, 0.25f))
		{
			if (! CheckState(EBaseStates::Stand))
			{
				Dash(BtnDashVector);
			}
		}
		else
		{
			BtnDashVector = forwardVector;
			SET_TIMER(BtnDashTimer, this, &AHeroBase::BtnDashReset, cTime(0.3));
		}
	} // Released End
	else
	{
		if (CheckState(EBaseStates::Stand) && FVector::PointsAreNear(BtnDashVector, forwardVector, 0.25f))
		{
			// PlayerCharacter->Sprint(BtnDashVector);
		}
	}
}

void AHeroBase::BtnTeleport_Implementation()
{
	Teleport();
}

//----------------------------------------------// Unit Interface Implementation
class UHeroStats* AHeroBase::GetHeroStatsI_Implementation()
{
	return GetHeroStats();
}

FName AHeroBase::GetUnitName_Implementation()
{
	return HeroName;
}

FName AHeroBase::GetFormName_Implementation()
{
	return GetHeroStats()->FormName;
}

bool AHeroBase::IsItHero_Implementation()
{
	return true;
}

bool AHeroBase::AddHealth_Implementation(float Value)
{
	GetHeroStats()->AddHealth(Value);
	return true;
}

bool AHeroBase::AddPower_Implementation(float Value)
{
	GetHeroStats()->AddPower(Value);
	return true;
}

bool AHeroBase::AddExp_Implementation(int32 Value)
{
	GetHeroStats()->AddExp(Value);
	return true;
}

void AHeroBase::UseLevelPoints_Implementation()
{
	uint8	Points = GetHeroStats()->GetLevelScore();
	FVector Stats  = FVector::ZeroVector;
	for (Points; Points > 0; Points--)
	{
		uint8 Rand = FMath::RandRange(0, 2);
		switch (Rand)
		{
		case 0: Stats += FVector(1.f, 0.f, 0.f); break;
		case 1: Stats += FVector(0.f, 1.f, 0.f); break;
		case 2: Stats += FVector(0.f, 0.f, 1.f); break;
		}
	}
	GetHeroStats()->AddStats(Stats, false);
}
