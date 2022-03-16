// Fill out your copyright notice in the Description page of Project Settings.

#include "Chars/Components/MonsterStats.h"

void UMonsterStats::Init()
{
	MaxHealth += MaxHealth * Level * 0.2f;
	Health = MaxHealth;
	Damage += Damage * Level * 0.2f;
	ExpForKill += (float)ExpForKill * Level * 0.2f;
}

float UMonsterStats::TakeDamage(float damage, float armorPiercing, bool block)
{
	Health -= damage;
	return damage;
}
