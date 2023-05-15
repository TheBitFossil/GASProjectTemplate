// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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


	/**	 Attribute Setters	**/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCurrentHealth(float GetHealth);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetMaxHealth(float GetMaxHealth);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCurrentMana(float GetMana);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetMaxMana(float GetMaxMana);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCurrentCharacterLevel(int32 INT32);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetStamina(float GetStamina);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetMaxStamina(float GetMaxStamina);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetStaminaRegen(float GetStaminaRegen);


	
};
