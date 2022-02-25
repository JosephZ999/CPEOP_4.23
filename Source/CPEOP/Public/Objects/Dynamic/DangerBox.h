// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "DangerBox.generated.h"

class USceneComponent;
class UBoxComponent;
class UStaticMeshComponent;

/**/
UCLASS()
class CPEOP_API ADangerBox : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(Category = Component, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* SceneRoot;

	UPROPERTY(Category = Component, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* Box;

	UPROPERTY(Category = Component, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Plane;

	EDangerPriority _Priority;
	uint8			_Team;
	float			_LifeTime;
	bool			_IsFading;
	bool			_IsVisible;

	FTimeline	 _CTimeline;
	UCurveFloat* _CurveFadeIn;
	UCurveFloat* _CurveFadeOut;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* _BoxMaterial;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* _SphereMaterial;

protected:
	virtual void BeginPlay() override;

public:
	// Sets default values for this actor's properties
	ADangerBox();

	virtual void Tick(float delta) override;

	UFUNCTION()
	void Init(uint8 Team, float LifeTime, bool ShowRegion, enum EDangerPriority Priority);

	UFUNCTION()
	void BeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	void OnOwnerStateChanged();

private:
	UFUNCTION()
	void FadeOut();

	UFUNCTION()
	void TimeLine(float Value);

public:
	// Blueprint functions
	UFUNCTION(BlueprintCallable)
	float GetLifeTime() const { return _LifeTime; }

	UFUNCTION(BlueprintImplementableEvent)
	void OnStartFade();
};
