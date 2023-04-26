// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player/GPlayerState.h"
#include "GUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYSYSTEM_API UGUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ShowAbilityConfirmCancelText(bool Message);
	void SetCurrentHealth(float GetHealth);
	void SetCurrentMaxHealth(float GetMaxHealth);
	void SetCurrentMana(float GetMana);
	void SetCurrentMaxMana(float GetMaxMana);
	void SetCurrentCharacterLevel(int32 INT32);
};
