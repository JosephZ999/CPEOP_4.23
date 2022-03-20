// Fill out your copyright notice in the Description page of Project Settings.

#include "Sys/MyPlayerController.h"
#include "GameFramework/HUD.h"

void AMyPlayerController::ShowGameUI_Implementation(bool Hide)
{
	IPlayerHUD::Execute_ShowGameUI(MyHUD, Hide);
}

void AMyPlayerController::UpdateLevel_Implementation(uint8 Level)
{
	IPlayerHUD::Execute_UpdateLevel(MyHUD, Level);
}

void AMyPlayerController::UpdateHp_Implementation(float Hp, float MaxHp)
{
	IPlayerHUD::Execute_UpdateHp(MyHUD, Hp, MaxHp);
}

void AMyPlayerController::UpdateMp_Implementation(float Mp, float MaxMp)
{
	IPlayerHUD::Execute_UpdateMp(MyHUD, Mp, MaxMp);
}

void AMyPlayerController::UpdateExp_Implementation(float Exp, float MaxExp)
{
	IPlayerHUD::Execute_UpdateExp(MyHUD, Exp, MaxExp);
}

void AMyPlayerController::UpdateStamina_Implementation(float Value)
{
	IPlayerHUD::Execute_UpdateStamina(MyHUD, Value);
}

void AMyPlayerController::ShowInputUI_Implementation(bool Hide)
{
	IPlayerHUD::Execute_ShowInputUI(MyHUD, Hide);
}

void AMyPlayerController::ShowWarningNotice_Implementation(bool Hide)
{
	IPlayerHUD::Execute_ShowWarningNotice(MyHUD, Hide);
}

void AMyPlayerController::ShowWaveScore_Implementation(int32 WaveNum, int32 TotalKills)
{
	IPlayerHUD::Execute_ShowWaveScore(MyHUD, WaveNum, TotalKills);
}

void AMyPlayerController::SkillActivated_Implementation()
{
	IPlayerHUD::Execute_SkillActivated(MyHUD);
}

void AMyPlayerController::SkillDeactivated_Implementation()
{
	IPlayerHUD::Execute_SkillDeactivated(MyHUD);
}

void AMyPlayerController::NotEnoughPower_Implementation()
{
	IPlayerHUD::Execute_NotEnoughPower(MyHUD);
}

void AMyPlayerController::NotEnoughStamina_Implementation()
{
	IPlayerHUD::Execute_NotEnoughStamina(MyHUD);
}
