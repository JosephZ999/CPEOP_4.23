// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sys/Interfaces/GameIns.h"
#include "Engine/GameInstance.h"
#include "Objects/Dynamic/DmgTextBase.h"

#include "MyGameInstance.generated.h"

/**
 *
 */
UCLASS()
class CPEOP_API UMyGameInstance : public UGameInstance, public IGameIns
{
	GENERATED_BODY()
};
