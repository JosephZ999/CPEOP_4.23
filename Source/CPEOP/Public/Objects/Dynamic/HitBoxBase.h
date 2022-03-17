// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Objects/Dynamic/Helper.h"
#include "Chars/UnitBase.h"
#include "Paper2D/Classes/PaperFlipbookComponent.h"

#include "HitBoxBase.generated.h"

UENUM()
enum class EDamageEffect : uint8
{
	Default,
	Frost,
	Fire,
};

UENUM()
enum class EImpulseType : uint8
{
	Rotation,
	// Location,
	OwnerLocation,
};

UCLASS(Blueprintable, Abstract)
class CPEOP_API AHitBoxBase : public AHelper
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHitBoxBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Helper Options")
	FHitOption Options;

	UPROPERTY(EditDefaultsOnly, Category = "Helper Options")
	EImpulseType ImpulseType;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Options //
public:
	UPROPERTY(EditDefaultsOnly, Category = "Helper Options")
	bool LoopAnim;

	UPROPERTY(BlueprintReadOnly)
	int32 HitCount;

private:
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	class AUnitBase* OwnerCharacter;

	// Range Attack
	UPROPERTY(EditDefaultsOnly, Category = "RandAttack")
	bool RandEnemy; // Range Attacks

	TArray<AUnitBase*> EnemiesInRange;
	FTimerHandle	   AttackTimer;

	UPROPERTY(EditDefaultsOnly, Category = "RandAttack")
	float AttackDelay;

	UFUNCTION()
	void AttackRandUnit();

	UFUNCTION()
	void Attack(AUnitBase* _Enemy);

public:
	virtual void Init(AUnitBase* owner, float damage, float crit) override;

	UFUNCTION()
	void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION(BlueprintNativeEvent)
	void OnHit(const int32& count, class AUnitBase* damagedUnit, bool attackBlocked);

	UFUNCTION(BlueprintCallable)
	void setSparkRotation(float rotation) { Options.sparkRotation = rotation; }
};
