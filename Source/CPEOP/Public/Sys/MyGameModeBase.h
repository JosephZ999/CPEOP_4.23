// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sys/Interfaces/GModeInterface.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

/**
 *
 */

UCLASS()
class CPEOP_API AMyGameModeBase : public AGameModeBase, public IGModeInterface
{
	GENERATED_BODY()
};
