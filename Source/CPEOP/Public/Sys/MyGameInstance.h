// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sys/Interfaces/GameIns.h"
#include "Engine/GameInstance.h"
#include "Objects/Dynamic/DmgTextBase.h"

#include "MyGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FDefFightData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ADmgTextBase> DamageText;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ADangerBox> DangerBox;

	UPROPERTY(EditDefaultsOnly)
	TArray<class UPaperFlipbook*> Sparks;
};

/**
 *
 */
UCLASS()
class CPEOP_API UMyGameInstance : public UGameInstance, public IGameIns
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Game Settings")
	FDefFightData FightClasses;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game Settings",
		Meta = (AllowPrivateAccess = "true", ClampMin = 0, ClampMax = 100))
	uint8 DamageTextRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game Settings", Meta = (AllowPrivateAccess = "true"))
	bool ShowCritDamageText;

public:
	// Interface function
	UFUNCTION(BlueprintNativeEvent, Category = "GameIns Events")
	class UPaperFlipbook* GetSparkAnimation(uint8 index);

	UFUNCTION(BlueprintNativeEvent, Category = "GameIns Events")
	TSubclassOf<class AHelper> GetDamageTextClass();

	UFUNCTION(BlueprintNativeEvent, Category = "GameIns Events")
	bool CanCreateDamageText(bool Crited);

	UFUNCTION(BlueprintNativeEvent, Category = "GameIns Events")
	class TSubclassOf<class ADangerBox> GetDangerBoxClass();
};
