// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "BCoreTypes.h"
#include "BSaveGame.generated.h"

/**
 *
 */
UCLASS()
class CPEOP_API UBSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	FGameSettings _GameSettings;
};
