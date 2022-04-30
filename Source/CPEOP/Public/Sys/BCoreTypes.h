#pragma once

#include "BCoreTypes.generated.h"

class AUnitBase;
class AHeroBase;
class AMonsterBase;
class ADmgTextBase;
class ADangerBox;
class UPaperFlipbook;
class APickUpBase;

// Game Mode Types

UENUM(BlueprintType)
enum class EGameResultType : uint8
{
	Win,
	Greate,
	Perfect,
	Lose,
};

USTRUCT(BlueprintType)
struct FGameResults
{
	GENERATED_BODY()

	FGameResults() {}

	UPROPERTY(BlueprintReadWrite)
	FText Title;

	UPROPERTY(BlueprintReadWrite)
	EGameResultType Result{EGameResultType::Win};

	UPROPERTY(BlueprintReadWrite)
	int32 Time{0};

	UPROPERTY(BlueprintReadWrite)
	int32 Kills{0};
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

USTRUCT(BlueprintType)
struct FPickUpParams
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<APickUpBase> Class;

	UPROPERTY(BlueprintReadWrite)
	FVector Location;

	UPROPERTY(BlueprintReadWrite)
	uint8 Level;

	UPROPERTY(BlueprintReadWrite, Meta = (ClampMin = 1, ClampMax = 10))
	uint8 Amount;

	UPROPERTY(BlueprintReadWrite)
	float Chance;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartBattleSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFinishBattleSignature, EGameResultType, Results);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitDeadSignature, AUnitBase*, Unit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeroLevelUpSignature, AHeroBase*, Hero);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWavePassedSignature);
