// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chars/Components/HeroStats.h"
#include "Components/TimelineComponent.h"
#include "sys/Interfaces/HeroInput.h"
#include "Chars/UnitBase.h"
#include "HeroBase.generated.h"

#define GET_STATS GetHeroStats()

class USceneComponent;
class USpringArmComponent;
class UCameraComponent;
class UHeroStats;
class UCurveFloat;

UENUM()
enum class ECameraMode : uint8
{
	Free,
	Action,
	Target,
};
enum EComboKey
{
	CK_None,
	CK_Attack,
	CK_AForward,
	CK_ABackward,
	CK_Jump,
	CK_Block,
	CK_Dash
};

/**
 *
 */
UCLASS()
class CPEOP_API AHeroBase : public AUnitBase, /* Interfaces: */ public IHeroInput
{
	GENERATED_BODY()

public:
	AHeroBase();

private:
	static FName ArmCompName;
	static FName CameraCompName;
	static FName HeroStatsCompName;
	FName		 HeroName;

private:
	/** Точка к которой будет двигаться камера с интерполяцтей */
	UPROPERTY(Category = Camera, VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	USceneComponent* CameraSceneComp;

	/** Точка к которой будет двигаться камера с интерполяцтей */
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* ArmComp;

	/** Камера компонент */
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComp;

	/** Характеристики персонажа */
	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UHeroStats* HeroStatsComp;

public:
	FORCEINLINE virtual UUnitStatsBase* GetUnitStats() const { return HeroStatsComp; }
	FORCEINLINE class UHeroStats*		GetHeroStats() const { return HeroStatsComp; }

	FVector GetCameraLocation();

protected:
	virtual void BeginPlay() override;
	virtual void EndState() override;

public:
	virtual void Tick(float delta) override;

	UFUNCTION(BlueprintImplementableEvent)
	void NotEnoughPower();
	UFUNCTION(BlueprintImplementableEvent)
	void NotEnoughStamina();

	void SetHeroName(FName NewName);

	//------------------------------------------// Timeline
protected:
	UCurveFloat* FindCurveFloat(FString path);
	void		 PlayTimeline(UObject* targetObject, UCurveFloat* curve, FName functionName, bool looping);
	void		 StopTimeline();
	FTimeline	 CurveTimeline;

	//------------------------------------------// Movement // Sprint // Dash //
public:
	void Sprint(FVector fwVector);
	void StopSprinting();
	void Dash(FVector fwVector);
	void DoDash();

	FORCEINLINE bool isSprinting() { return Sprinting; }

private:
	FVector		 DashVector;
	bool		 Sprinting;
	FTimerHandle SprintPowReducingTimer;
	void		 SprintPowReducing();

	//------------------------------------------// Blocking Movement
private:
	virtual void	 Move() override;
	FORCEINLINE void UpdateAnim();

	UFUNCTION()
	void OnCompHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
		const FHitResult& Hit);

	void LockMovement(FVector Point);
	void UnlockMovementX();
	void UnlockMovementY();

	bool LockUp		 = false;
	bool LockDown	 = false;
	bool LockForward = false;
	bool LockBack	 = false;

	//------------------------------------------// Camera Behaviour
private:
	ECameraMode CameraMode = ECameraMode::Free;

	// Calls in every frame
	FORCEINLINE void UpdateCameraView(float delta);

public:
	// Free Mode
	UFUNCTION(BlueprintCallable)
	void SetCameraViewF(float CameraStartLoc, float CameraEndLoc);

	UFUNCTION(BlueprintCallable)
	void SetCameraClampY(float ClampA, float ClampB)
	{
		CameraYClampA = ClampA;
		CameraYClampB = ClampB;
	}

private:
	float CameraXClampA = -200; // Camera view point A in Free Mode
	float CameraXClampB = 200;	// Camera view point B in Free Mode
	float CameraYClampA = -500.f;
	float CameraYClampB = 500.f;

	// Action Mode
public:
	UFUNCTION(BlueprintCallable)
	void SetCameraViewA(FVector CameraLocation, float Duration, ECameraMode NextCamMode = ECameraMode::Free);

private:
	void		 DisableCameraViewA();
	FVector		 CameraView{FVector::ZeroVector}; // Camera view in Action Mode
	ECameraMode	 CameraLastMode = ECameraMode::Free;
	FTimerHandle CamActionTimer;

public:
	// Target Mode
	UFUNCTION(BlueprintCallable)
	void SetCameraTarget(AUnitBase* target, float dist = 350.f);

	UFUNCTION()
	const FVector& GetCameraViewPosition() const { return CameraView; }

private:
	UPROPERTY()
	AUnitBase* CameraTargetActor;
	float	   CameraTargetDist;

	//------------------------------------------// Actions
public:
	virtual void Attack();
	virtual void AttackHold();
	virtual void AttackBack();
	virtual void AttackForward();
	virtual void AttackDown() {}
	void		 Block();
	void		 BlockStop();
	FTimerHandle BlockTimer;

	UFUNCTION(BlueprintCallable)
	void Bp_Block() { Block(); }

	void		 PowCharge();
	void		 PowChargeLoop();
	void		 PowChargeEnd();
	FTimerHandle PowChargeTimer;
	FTimerHandle PowChargeLoopTimer;

	void SkillEnable();

	UFUNCTION(BlueprintImplementableEvent)
	void HeroSkillActivated();
	UFUNCTION(BlueprintImplementableEvent)
	void HeroSkillDeactivated();

	void SkillDisable();
	void SkillCanceled();
	bool IsSkillActive() const { return Skill; }

private:
	FTimerHandle skillEnTimer;
	FTimerHandle skillDisTimer;
	bool		 Skill;

	//------------------------------------------// Actions Combination
private:
	FTimerHandle	  ComboTimer;
	TArray<EComboKey> ComboKeys;
	bool			  ComboSuccess;
	uint8			  KeyIndex;

public:
	void			 Combo(float time);
	virtual void	 ComboI();
	FORCEINLINE bool isComboTime() const { return ComboSuccess; }

	void addKey(EComboKey key);

	UFUNCTION(BlueprintCallable, Category = "Actions Combo")
	void ResetKeys();

protected:
	EComboKey getNextKey();

	//------------------------------------------// Teleport
public:
	bool Teleport();
	void Teleport(FVector nLocation);

private:
	void	TeleportTick(float delta);
	FVector tp_initialLocation;
	FVector tp_Vector;
	float	tp_DistPassed;
	float	tp_MaxDist;

	//------------------------------------------// Transformation
public:
	UFUNCTION(BlueprintCallable)
	void ChangeForm(FName formName);
	void InitForm(FName formName, FVector stats);

	//------------------------------------------// AI Interface

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AI Interface")
	void SetAIEnabled(bool Enable);

	//------------------------------------------// Hero Inputs
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Hero Commands")
	void BtnSetMovement(FVector Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Hero Commands")
	void BtnAction(EInputActionType action, bool btnReleased);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Hero Commands")
	void BtnDash(FVector forwardVector, bool Released);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Hero Comnnads")
	void BtnTeleport();

	//------------------------------------------// Unit Interface

public:
	// UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Unit Interface", Meta = (Keywords = "Get"))
	// UUnitStats* GetUnitStatsI();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Unit Interface", Meta = (Keywords = "Get"))
	UHeroStats* GetHeroStatsI();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Unit Interface", Meta = (Keywords = "Get"))
	FName GetUnitName();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Unit Interface", Meta = (Keywords = "Get"))
	FName GetFormName();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Unit Interface", Meta = (Keywords = "Hero"))
	bool IsItHero();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Unit Interface", Meta = (Keywords = "Add"))
	bool AddHealth(float Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Unit Interface", Meta = (Keywords = "Add"))
	bool AddPower(float Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Unit Interface", Meta = (Keywords = "Add"))
	bool AddExp(int32 Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Unit Interface", Meta = (Keywords = "Add"))
	void UseLevelPoints();
};
