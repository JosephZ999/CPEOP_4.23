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

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ADmgTextBase> DamageTextClass;
};

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

// Save Load Types
USTRUCT(BlueprintType)
struct FGameSettings
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	bool bAlwaysShowCritText = true;

	UPROPERTY(BlueprintReadWrite)
	uint8 CrossDevider = 8;

	UPROPERTY(BlueprintReadWrite)
	uint8 DamageTextRate = 20;

	FORCEINLINE bool CanCreateDamageText(bool Crited);
};

FORCEINLINE bool FGameSettings::CanCreateDamageText(bool Crited)
{
	if (Crited && bAlwaysShowCritText) { return true; }
	else
	{
		return FMath::RandRange(1, 100) <= DamageTextRate;
	}
}

UENUM(BlueprintType)
enum class EGameSettingsKeys : uint8
{
	None,
	CrossDevisions,
	DamageTextRate,
	ShowCritText,
	Max,
};

UENUM(BlueprintType)
enum class EGameOptionValueType : uint8
{
	None,
	Bool,
	Float,
	Uint8,
};

USTRUCT(BlueprintType)
struct FGameOptionValue
{
	GENERATED_BODY()

	explicit FGameOptionValue()
		: Type(EGameOptionValueType::None)
		, ValueType1(false)
		, ValueType2(0.f)
		, ValueType3(0)
	{
	}

	explicit FGameOptionValue(bool nValue)
		: Type(EGameOptionValueType::Bool)
		, ValueType1(nValue)
		, ValueType2(float(nValue))
		, ValueType3(uint8(nValue))
	{
	}

	explicit FGameOptionValue(float nValue)
		: Type(EGameOptionValueType::Float)
		, ValueType1(nValue)
		, ValueType2(float(nValue))
		, ValueType3(uint8(nValue))
	{
	}

	explicit FGameOptionValue(uint8 nValue)
		: Type(EGameOptionValueType::Uint8)
		, ValueType1(nValue)
		, ValueType2(float(nValue))
		, ValueType3(uint8(nValue))
	{
	}

	UPROPERTY(BlueprintReadWrite)
	EGameOptionValueType Type;

	UPROPERTY(BlueprintReadWrite)
	bool ValueType1;

	UPROPERTY(BlueprintReadWrite)
	float ValueType2;

	UPROPERTY(BlueprintReadWrite)
	uint8 ValueType3;
};
