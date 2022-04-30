// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "GameFramework/HUD.h"

void AMyPlayerController::ShowGameUI_Implementation(bool Hide)
{
	IPlayerHUD::Execute_ShowGameUI(MyHUD, Hide);
}

void AMyPlayerController::ShowPauseUI_Implementation(bool Hide)
{
	IPlayerHUD::Execute_ShowPauseUI(MyHUD, Hide);
}

void AMyPlayerController::LevelUpdated_Implementation(uint8 Level)
{
	IPlayerHUD::Execute_LevelUpdated(MyHUD, Level);
}

void AMyPlayerController::HpUpdated_Implementation(float Hp, float MaxHp)
{
	IPlayerHUD::Execute_HpUpdated(MyHUD, Hp, MaxHp);
}

void AMyPlayerController::MpUpdated_Implementation(float Mp, float MaxMp)
{
	IPlayerHUD::Execute_MpUpdated(MyHUD, Mp, MaxMp);
}

void AMyPlayerController::ExpUpdated_Implementation(float Exp, float MaxExp)
{
	IPlayerHUD::Execute_ExpUpdated(MyHUD, Exp, MaxExp);
}

void AMyPlayerController::StaminaUpdated_Implementation(float Value)
{
	IPlayerHUD::Execute_StaminaUpdated(MyHUD, Value);
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
