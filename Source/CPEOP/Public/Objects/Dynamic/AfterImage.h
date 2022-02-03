// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"

#include "AfterImage.generated.h"

class UPaperFlipbook;
class UPaperFlipbookComponent;

/**
 * 
 */
UCLASS()
class CPEOP_API AAfterImage : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	UPaperFlipbookComponent* Sprite;

public:
	AAfterImage();

	void Init(UPaperFlipbook* Anim, int8 Frame=0, bool OneFrame=true, bool LoopAnim=false);

	FTimeline CurveTimeline;

	UPROPERTY(EditDefaultsOnly)
	class UCurveFloat* FadeTimeLineCurve;
	UPROPERTY(EditDefaultsOnly)
	class UCurveLinearColor* FadeColorCurve;

public:
	virtual void Tick(float delta) override;

private:
	UFUNCTION()
	void Fade(float value);

protected:
	virtual void BeginPlay() override;
};
