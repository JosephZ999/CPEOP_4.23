// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Objects/Dynamic/DmgTextBase.h"

#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CPEOP_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess = "true"))
	TArray<class UPaperFlipbook*> Data_Sparks;

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ADmgTextBase> DamageText;

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess = "true", ClampMin = 0, ClampMax = 100))
	uint8 DamageTextRate;

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess = "true"))
	bool ShowCritDamageText;

	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess = "true"))
	class TSubclassOf<AActor> ShadowClass;

public:
	UFUNCTION()
	class UPaperFlipbook* getSpark(uint8 index) const;
	TSubclassOf<class ADmgTextBase> getDamageTextClass() const;
	bool canCreateDamageText(bool crited)const;
	TSubclassOf<AActor> getShadowClass() const { return ShadowClass; }

	//==========================================/ Hero Events
	UFUNCTION(BlueprintImplementableEvent, Category = "Units Calls")
	void UnitKilled(class AUnitBase* killerUnit, class AUnitBase* killedUnit);

};
