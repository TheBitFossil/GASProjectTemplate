// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GPlayerController.h"

#include "AbilitySystemComponent.h"
#include "Player/GPlayerState.h"
#include "UI/GUserWidget.h"

void AGPlayerController::CreateHUD()
{
	if(HUDWidget)
	{
		return;
	}

	if(!HUDWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing HUDWidgetClass. Please fill in on the BP of the PlayerController"));
		return;
	}

	// Only create HUD on the local client
	if(false == IsLocalPlayerController())
	{
		return;
	}

	// Need a valid PlayerState to get Attributes from
	AGPlayerState* PlayerState = GetPlayerState<AGPlayerState>();
	if(!PlayerState)
	{
		return;
	}

	// Create the HUD into the viewport
	HUDWidget = CreateWidget<UGUserWidget>(this, HUDWidgetClass);
	HUDWidget->AddToViewport();
	
	// Set Attributes
	HUDWidget->SetCurrentHealth(PlayerState->GetHealth());
	HUDWidget->SetMaxHealth(PlayerState->GetMaxHealth());
	HUDWidget->SetCurrentMana(PlayerState->GetMana());
	HUDWidget->SetMaxMana(PlayerState->GetMaxMana());
	HUDWidget->SetCurrentCharacterLevel(PlayerState->GetCharacterLevel());
	HUDWidget->SetCurrentStamina(PlayerState->GetStamina());
	HUDWidget->SetMaxStamina(PlayerState->GetMaxStamina());
	HUDWidget->SetStaminaRegen(PlayerState->GetStaminaRegen());
	
}

UGUserWidget* AGPlayerController::GetHUD()
{
	return HUDWidget;
}

void AGPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AGPlayerState* PlayerState = GetPlayerState<AGPlayerState>();
	if(PlayerState)
	{
		// Init ASC with PState (owner) and our new Pawn (Avatar Actor)
		PlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(PlayerState, InPawn);
	}
}

void AGPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Safety Net: when state is replicated before the hero is possessed
	CreateHUD();
}
