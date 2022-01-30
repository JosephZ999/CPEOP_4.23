// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chars/MonsterBase.h"
#include "Shrieker.generated.h"

/**
 * 
 */
UCLASS()
class CPEOP_API AShrieker : public AMonsterBase
{
	GENERATED_BODY()

public:
	AShrieker();

	virtual void Attack() override;
	
	virtual void AnimUpdate() override;
	virtual void EndState() override;
	virtual void Landed(const FHitResult& Hit) override;

	UFUNCTION()
	void Summon();

	UFUNCTION(BlueprintCallable)
	AShriekerSummon * SpawnSummon(TSubclassOf<class AShriekerSummon> Class, FVector Location);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_Summon();

	uint8 SummonsNum;
	TArray<class AShriekerSummon*> SummonArr;

	UFUNCTION(BlueprintCallable)
	void SetSummonsNum(uint8 Num);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	uint8 GetSummonsNum() { return SummonsNum; }

protected:
	virtual void BeginPlay() override;

};
