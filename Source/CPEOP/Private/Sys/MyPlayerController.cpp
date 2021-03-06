// Fill out your copyright notice in the Description page of Project Settings.


#include "Sys/MyPlayerController.h"
#include "kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Engine/Classes/Engine/Engine.h"
#include "Chars/HeroBase.h"
#include "TimerManager.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
}


bool AMyPlayerController::SpawnPlayerCharacter(TSubclassOf<AHeroBase> hero)
{
	AActor* playerStart = UGameplayStatics::GetActorOfClass(this, APlayerStart::StaticClass());
	if (playerStart)
	{
		FTransform nT(playerStart->GetTransform());
		nT.SetScale3D(FVector::OneVector);

		AHeroBase* character = GetWorld()->SpawnActorDeferred<AHeroBase>(hero, nT);
		if (character)
		{
			Possess(character);
			UGameplayStatics::FinishSpawningActor(character, nT);
			PlayerCharacter = character;
			return true;
		}
	}
	return false;
}


//////////////////////// Input ////////////////////////////

void AMyPlayerController::BtnDash(FVector forwardVector, bool Released)
{
	if (Released)
	{
		// Stop Sprinting
		if (PlayerCharacter->isSprinting())
		{
			PlayerCharacter->StopSprinting();
			return;
		}

		// Set Dash Forward Vector
		if (DashVector.IsNearlyZero())
		{
			DashVector = forwardVector;
			GetWorldTimerManager().SetTimer(DashTimer, this, &AMyPlayerController::DashDisable, PlayerCharacter->cTime(0.3));
			return;
		}
		else
		{
			if (!DashVector.Equals(forwardVector))
			{
				DashVector = forwardVector;
				GetWorldTimerManager().SetTimer(DashTimer, this, &AMyPlayerController::DashDisable, PlayerCharacter->cTime(0.3));
				return;
			}
		}

		if (!PlayerCharacter->checkState(EBaseStates::Stand))
		{
			PlayerCharacter->Dash(DashVector);
		}
	}	// Released End
	else
	{
		if (PlayerCharacter->checkState(EBaseStates::Stand) && DashVector.Equals(forwardVector))
		{
			// PlayerCharacter->Sprint(DashVector);
		}
	}
}

void AMyPlayerController::DashDisable()
{
	DashVector = FVector::ZeroVector;
}

void AMyPlayerController::BtnAction(EInputActionType action, bool btnReleased)
{
	switch (action)
	{
	case EInputActionType::A_Center:
	{
		if (btnReleased)	
		{
			PlayerCharacter->Attack(); 
			PlayerCharacter->addKey(EComboKey::CK_Attack);
		}
		else				
		{ 
			PlayerCharacter->AttackHold();	
		}
		break;
	}
	case EInputActionType::A_Up:
	{
		if (btnReleased)
		{
			PlayerCharacter->EventJump();
			PlayerCharacter->addKey(EComboKey::CK_Jump);
		}
		break;
	}
	case EInputActionType::A_Down:
	{
		if (btnReleased)
		{
			PlayerCharacter->BlockStop();
			PlayerCharacter->addKey(EComboKey::CK_Block);
		}
		break;
	}
	case EInputActionType::A_Forward:
	{
		if (btnReleased)
		{
			if (PlayerCharacter->isLookingRight())	
			{ 
				PlayerCharacter->AttackForward();
				PlayerCharacter->addKey(EComboKey::CK_AForward);
			}
			else									
			{ 
				PlayerCharacter->AttackBack();	
				PlayerCharacter->addKey(EComboKey::CK_ABackward);
			}
		}
		break;
	}
	case EInputActionType::A_Backward:
	{
		if (btnReleased)
		{
			if (PlayerCharacter->isLookingRight())	
			{ 
				PlayerCharacter->AttackBack();	
				PlayerCharacter->addKey(EComboKey::CK_ABackward);
			}
			else
			{
				PlayerCharacter->AttackForward();
				PlayerCharacter->addKey(EComboKey::CK_AForward);
			}
		}
		break;
	}

	case EInputActionType::A_BlockStart:		{ PlayerCharacter->Block(); PlayerCharacter->SkillCanceled();		break; }
	case EInputActionType::A_BlockEnd:			{ PlayerCharacter->BlockStop();	break; }
	} // End switch

	if (btnReleased)
	{
		PlayerCharacter->PowChargeEnd();
		PlayerCharacter->SkillCanceled();
	}
}
