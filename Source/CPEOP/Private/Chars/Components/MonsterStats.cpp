// Fill out your copyright notice in the Description page of Project Settings.


#include "Chars/Components/MonsterStats.h"


void UMonsterStats::Init(uint8 level)
{
	level = FMath::Clamp((int32)level, 0, 100);

	MaxHealth += MaxHealth * level * 0.2f;
	Health = MaxHealth;
	Damage += Damage * level * 0.2f;
}

float UMonsterStats::TakeDamage(float damage, bool block)
{
	Health -= damage;
	return damage;
}