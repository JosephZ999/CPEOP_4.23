// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Helper.generated.h"

UENUM()
enum class EHelperType : uint8
{
	None,
	HitBox,
	HitSpark,
	SpecialEffect,
	DmgText,
};

UCLASS(Abstract)
class CPEOP_API AHelper : public AActor
{
	GENERATED_BODY()

public:
	static FName SpriteName;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	class UPaperFlipbookComponent* Sprite;

public:
	FORCEINLINE class UPaperFlipbookComponent* GetSprite() const { return Sprite; }

public:
	// Sets default values for this actor's properties
	AHelper(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(VisibleAnywhere, Category = "Helper Options")
	EHelperType Type;

	UPROPERTY(EditDefaultsOnly, Category = "Helper Options")
	bool bAttachToParent;

	UPROPERTY(EditDefaultsOnly, Category = "Helper Options")
	bool bAttachRotation; // Attach rotation to parent actor rotation

	UPROPERTY(EditDefaultsOnly, Category = "Helper Options")
	FVector InitialLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// HitBox Initialization
	virtual void Init(class AUnitBase* owner, float damage, float crit){};
	// DmgText Init
	virtual void Init(float damage, bool crit, bool isMovingRight){};
	// Spark Init
	virtual void Init(class UPaperFlipbook* anim, float rotation){};

	virtual void OnOwnerStateChanged();
};
