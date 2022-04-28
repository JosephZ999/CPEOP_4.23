#pragma once

#include "BCoreTypes.generated.h"

// Game Mode Types

class AUnitBase;
class AHeroBase;
class AMonsterBase;
class AMyPlayerController;
class APickUpBase;
class ADmgTextBase;
class ADangerBox;
class UPaperFlipbook;

UENUM(BlueprintType)
enum class EGameResultType : uint8
{
	Win,
	Greate,
	Perfect,
	Lose,
};

UENUM(BlueprintType)
enum class EAttackSlashType : uint8
{
	AST_Blue,
	AST_Green,
	AST_Red,
};

USTRUCT(BlueprintType)
struct FBGameData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ADangerBox> DangerBoxClass;

	UPROPERTY(EditDefaultsOnly)
	TArray<UPaperFlipbook*> SparkAnims;

	// Damage floating text

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ADmgTextBase> DamageTextClass;

	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = "0", ClampMax = "100"))
	uint8 DamageTextRate;

	UPROPERTY(EditDefaultsOnly)
	bool AlwaysShowCritText;

	FORCEINLINE bool CanCreateDamageText(bool Crited);
};

FORCEINLINE bool FBGameData::CanCreateDamageText(bool Crited)
{
	if (Crited && AlwaysShowCritText) { return true; }
	else
	{
		return FMath::RandRange(1, 100) <= DamageTextRate;
	}
}

USTRUCT(BlueprintType)
struct FSpawnParams
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FTransform Transform;

	UPROPERTY(BlueprintReadWrite)
	uint8 Team;

	UPROPERTY(BlueprintReadWrite)
	uint8 Level = 1;
};

USTRUCT(BlueprintType)
struct FWaveMonster
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<AMonsterBase> MonsterClass;

	UPROPERTY(BlueprintReadWrite)
	int32 Number;

	UPROPERTY(BlueprintReadWrite)
	uint8 Level;

	UPROPERTY(BlueprintReadWrite)
	uint8 LevelAdd;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartBattleSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFinishBattleSignature, EGameResultType, Results);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitDeadSignature, AUnitBase*, Unit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeroLevelUpSignature, AHeroBase*, Hero);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWavePassedSignature);
