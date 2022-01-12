// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"

#include "TimerManager.h"
#include "UnitBase.generated.h"

#define SET_TIMER	  GetWorldTimerManager().SetTimer
#define PAUSE_TIMER   GetWorldTimerManager().PauseTimer

/**
 * 
 */
class UPaperFlipbook;
class AHitBoxBase;

USTRUCT()
struct FHelperInfo
{
	GENERATED_BODY()

	FHelperInfo(uint16 inState, FName inName, FRotator inRotation, FVector inScale, float inTime)
		: state(inState)
		, name(inName)
		, rotation(inRotation)
		, scale(inScale)
		, time(inTime)
	{}

	FHelperInfo() {}

	uint16		state;
	FName		name;
	FRotator	rotation;
	FVector		scale;
	float		time;

	friend bool operator<(const FHelperInfo& A, const FHelperInfo& B)
	{
		return A.time < B.time;
	}
	friend bool operator>(const FHelperInfo& A, const FHelperInfo& B)
	{
		return B.time < A.time;
	}
	
};

USTRUCT(BlueprintType)
struct FHitOption
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)		float		damage;
	UPROPERTY(EditDefaultsOnly)		float		critRate;
	UPROPERTY(EditDefaultsOnly)		FVector2D	impulse;
	UPROPERTY(EditDefaultsOnly)		uint8		sparkIndex;
	UPROPERTY(EditDefaultsOnly)		FVector2D	sparkScale;
	UPROPERTY(EditDefaultsOnly)		float		sparkRotation;
	UPROPERTY(EditDefaultsOnly)		bool		fall;

	bool isCriticalDamage()
	{
		return FMath::FRandRange(0.f, 100.f) <= critRate;
	}

};

UENUM(BlueprintType)
enum EBaseStates
{
	Stand,
	JumpStart,
	Jumping,
	JumpLand,
	Blocking,
	Hit,
	Fall,
	StandUp,
	Dash,

	PowCharge,
	PowChargeLoop,
	PowChargeEnd,

	Teleport,

	LastIndex,
};

UENUM()
enum class EBlockType : uint8 { None, Forward, Back, Both };

UENUM()
enum class EDangerType : uint8 { None, MeleeAttack, DistanceAttack, };

UCLASS()
class CPEOP_API AUnitBase : public APaperCharacter
{
	GENERATED_BODY()

public:
	AUnitBase();

	class UMyGameInstance* GameInsRef;

protected:	virtual void BeginPlay();
public:		virtual void Tick(float delta);

	bool    Control = true;		// Запрещает любые движение
private:
// Variables //
	uint8   State{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	uint8   Team{ 0 };

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	bool    Dead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	class UShadowComponent* ShadowComp;

protected:
	bool CanFall{ true };

public:
// AI
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = AI)
	void    StartAI();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = AI)
	void    StopAI();

// Getters and Setters //
	UFUNCTION(BlueprintCallable)
	bool	checkState(uint8 nState) const		{ return State == nState;	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE uint8 getState() 		        { return State;				}

	bool	checkTeam(uint8 nTeam)				{ return Team == nTeam;		}

	void	setTeam(uint8 nTeam)				{ Team = nTeam; }

	FORCEINLINE const uint8& getTeam()			{ return Team;				}

	UFUNCTION(BlueprintCallable)
	virtual class UUnitStatsBase* getStatsComp() const { return nullptr; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UShadowComponent* getShadow() const { return ShadowComp; }

// Conditions //
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual bool IsDead()							{ return Dead; }

	/* Условия блокировки атак */
	FORCEINLINE virtual bool isBlockingAttack()		{ return State == EBaseStates::Blocking; }

	/* Смотрит вправо */
	FORCEINLINE virtual bool isLookingRight()		{ return int(GetActorRotation().Yaw) == 0; }

	FORCEINLINE bool isMovingRight()				{ return MoveVector.X > 0.f; }

	FORCEINLINE virtual bool isBeginAttack()		{ return false; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool isFalling() const { return State == EBaseStates::Fall; }

// Functions //----------------------------------
	void FindHelper(FString objectPath, TSubclassOf<class AHelper>& Class);
	UPaperFlipbook* FindAnim(FString objectPath);

	/* time / custom time delation */
	FORCEINLINE float cTime(float time)	{ return time / CustomTimeDilation; }

	/* Get Animation frame time */
	float getFrameTime(uint8 frame);



// Movement //===================================------------------------------
protected:
	virtual void Move();
	FVector MoveVector;

public:
	/* Impulse */
	void AddImpulse(FVector2D impulse, float time=0.f);
	void AddImpulse(FVector impulse, float time=0.f);
private:
	void ImpulseDeferred();
	FTimerHandle ImpulseTimer;
	FVector ImpulseVector;
public:
	UFUNCTION(BlueprintCallable)
	void SetMoveVector(FVector nVec = FVector::ZeroVector);
	UFUNCTION(BlueprintCallable)
	const FVector& GetMoveVector() const { return MoveVector; }

	void SetRotation(bool right, bool moveVec=true);

	// Actions
	UFUNCTION(BlueprintCallable)
	void EventJump();
	void Jumping();

	virtual void Landed(const FHitResult& Hit) override;
// End Movement //===============================------------------------------

// Helper //=====================================------------------------------
protected:
	/*Добавляет класс "HitBox" в массив компонента "HitComp" 
	* Пример: InitHelper("Attack_1, "Blueprint/HitBox/Attack_1")*/
	void InitHelper(FName name, FString classPath = FString());

	/* Собирает данные о создаваемом обьекте и запускает таймер для спавна */
	void SpawnHelper(FName name, float time = 0.f, FRotator rotation = { 0.f, 0.f, 0.f }, FVector scale = FVector::OneVector);
private:
	/* Список всех обьектов 'HitBox' */
	UPROPERTY(EditDefaultsOnly, Category = "UnitOptions|Data")
	TMap<FName, TSubclassOf<class AHelper>> HelpersData;

	/* Сортированный список обьектов 'HitBox' для спавна*/
	TArray<FHelperInfo> HelpersOrder;

	/* Сортировка создаваемых обьектов и спавн */
	void HelperSort();

	/* */
	void HelperSpawning(FHelperInfo info, TSubclassOf<class AHelper> hitClass);

// End Helper //=================================------------------------------


// Taking Damage //==============================------------------------------
public:
	/* On Damaged */
	void ApplyDamage(class AUnitBase* damageCauser, FHitOption* damageOption, bool fromBehind);
	virtual void OnDamaged() {}
	
	// Blocking
protected:
	EBlockType BlockAttackType;
	void SetBlockingAttack(EBlockType type, float start, float end );
	void SetBlockType();
	EBlockType nBlockT;
	void DisableBlocking();
	FTimerHandle blockTimer;
	FTimerHandle blockEndTimer;

	// Falling
	void Fall();
	void FallDeferred(float time);
	FTimerHandle fallTimer;
public:
	void StandUp();
private:
	void CreateSpark(uint8 index, FVector2D scale, float rotation);	
	void CreateDamageText(float damage, bool moveRight, bool crit);

public:
	UFUNCTION(BlueprintCallable)
	void BP_Fall()
	{
		Fall();
	}
// End Taking Damage //==========================------------------------------

// State Type //=================================------------------------------
protected:
	UFUNCTION(BlueprintCallable)
	void NewState(uint8 state, FName anim, uint8 frame=0, bool ctrl=false, bool endState=true);
	void NewState(uint8 state);
	
protected:
	void EndStateDeferred(float time);
	FTimerHandle EndStateTimer;
	virtual void EndState();

	// Animations
	TMap<FName, class UPaperFlipbook*>* AnimData{ nullptr };
	void AddAnimation(FName name, FString flipbookPath);
	void SetAnim(UPaperFlipbook* anim, bool playFromStart);
	void SetAnim(FName name, bool playFromStart);
	UPaperFlipbook* GetAnim(FName AnimName);
	float AnimElemTime(uint8 frame);
	

// End State Type //=============================------------------------------
	
//---------------------------------------------// Attack Danger Notification
protected:
	/* Attack Danger Notification */
	void DangerN(float duration, EDangerType type); 
	void DangerNFinish();
	FTimerHandle DamageNTimer;

	/* Attack Danger Type */
	EDangerType DangerNoticeType;

public:
	EDangerType GetDangerType() { return DangerNoticeType; }
};
