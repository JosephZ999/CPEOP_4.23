// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Chars/Components/HeroStats.h"

#include "CoreMinimal.h"
#include "Chars/UnitBase.h"
#include "HeroBase.generated.h"

#define GET_STATS getHeroStatsComp()

/**
 * 
 */

enum EComboKey { CK_None, CK_Attack, CK_AForward, CK_ABackward, CK_Jump, CK_Block, CK_Dash };

UCLASS()
class CPEOP_API AHeroBase : public AUnitBase
{
	GENERATED_BODY()

	// Лучше переименовать тип указателей на функции
	typedef void(*FuncPtr)(void);

public:
	AHeroBase();
private:
	static FName ArmCompName;
	static FName CameraCompName;
	static FName HeroStatsCompName;

private:
	/** Точка к которой будет двигаться камера с интерполяцтей */
	UPROPERTY(Category = Camera, VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
	class USceneComponent* CameraSceneComp; 

	/** Точка к которой будет двигаться камера с интерполяцтей */
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* ArmComp;

	/** Камера компонент */
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComp;

	/** Характеристики персонажа */
	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	class UHeroStats* HeroStatsComp;

	UPROPERTY(Category = Components, VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	class AMyPlayerController* PlayerController;

public:
	AMyPlayerController* getController();

	FORCEINLINE virtual class UUnitStatsBase* getStatsComp() const { return HeroStatsComp; }
	FORCEINLINE class UHeroStats* getHeroStatsComp()         const { return HeroStatsComp; }
protected:
	virtual void BeginPlay()		override;
	virtual void EndState()			override;
public:
	virtual void Tick(float delta)	override;

//---------------------------------------------// Movement // Sprint // Dash //
public:
	void Sprint	(FVector fwVector);
	void StopSprinting();
	void Dash	(FVector fwVector);
	void DoDash();
	FORCEINLINE bool isSprinting() { return Sprinting; }

private:
	FVector DashVector;
	bool Sprinting;
	FTimerHandle SprintPowReducingTimer;
	void SprintPowReducing();
//---------------------------------------------// End

//---------------------------------------------// Blocking Movement
private:
	virtual void Move() override;
	FORCEINLINE void UpdateAnim();

	UFUNCTION()
	void OnCompHit(
		UPrimitiveComponent*	HitComponent,
		AActor*					OtherActor,
		UPrimitiveComponent*	OtherComp,
		FVector					NormalImpulse,
		const FHitResult&		Hit
	);

	void LockMovement(FVector Point);
	void UnlockMovementX();
	void UnlockMovementY();

	bool LockUp			= false;
	bool LockDown		= false;
	bool LockForward	= false;
	bool LockBack		= false;
//---------------------------------------------// End

//---------------------------------------------// Camera Behaviour
private:
	enum class ECameraMode
	{
		Free,
		Action,
		Target,
	};

	ECameraMode CameraMode = ECameraMode::Free;

	// Calls in every frame
	FORCEINLINE void UpdateCameraView(float delta); 

public:
	// Free Mode
	UFUNCTION(BlueprintCallable)
	void SetCameraViewF(float CameraStartLoc, float CameraEndLoc);
private:
	float CameraXClampA = -200; // Camera view point A in Free Mode
	float CameraXClampB = 200; // Camera view point B in Free Mode

public:
	// Action Mode
	UFUNCTION(BlueprintCallable)
	void SetCameraViewA(FVector CameraLocation);
private:
	FVector CameraView = { FVector::ZeroVector }; // Camera view in Action Mode

public:
	// Target Mode
	UFUNCTION(BlueprintCallable)
	void SetCameraTarget(APawn* target);
private:
	APawn* CameraTargetActor;

//---------------------------------------------// End


//---------------------------------------------// Actions
public:
	virtual void Attack();
	virtual void AttackHold();
	virtual void AttackBack();
	virtual void AttackForward();
	virtual void Block();
	virtual void BlockStop();
	FTimerHandle BlockTimer;

	UFUNCTION(BlueprintCallable)
		void Bp_Block() { Block(); }

	void PowCharge();
	void PowChargeLoop();
	void PowChargeEnd();
	FTimerHandle PowChargeTimer;
	FTimerHandle PowChargeLoopTimer;

	void SkillEnable();
	void SkillDisable();
	void SkillCanceled();
	bool SkillisActive()const { return Skill; }
private:
	FTimerHandle skillEnTimer;
	FTimerHandle skillDisTimer;
	bool Skill;
//---------------------------------------------// End

//---------------------------------------------// Actions Combination
private:
	FTimerHandle		ComboTimer;
	TArray<EComboKey>	ComboKeys;
	bool				ComboSuccess;
	uint8				KeyIndex;

public:
	void Combo(float time);
	virtual void ComboI();
	FORCEINLINE bool isComboTime() const { return ComboSuccess; }

	void addKey(EComboKey key);

	UFUNCTION(BlueprintCallable, Category = "Actions Combo")
	void resetKeys();
protected:
	EComboKey getNextKey();
//---------------------------------------------// End

//---------------------------------------------// Teleport
public:
	UFUNCTION(BlueprintCallable)
	void Teleport();
	void Teleport(FVector nLocation);

private:
	inline void TeleportTick(float delta);
	FVector tp_initialLocation;
	FVector tp_Vector;
	float tp_DistPassed;
	float tp_MaxDist;
//---------------------------------------------// End

//---------------------------------------------// Transformation
public:
	UFUNCTION(BlueprintCallable)
	void ChangeForm(FName formName);
	void InitForm(FName formName, FVector stats);
//---------------------------------------------// End
};