// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CharacterAbilitySystemComponent.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE_ThreeParams(FReceivedDamageDelegate
										,UCharacterAbilitySystemComponent* SourceASC
										,float UnMitigatedDamage
										,float MitigatedDamage
										);

UCLASS()
class GAMEPLAYSYSTEM_API UCharacterAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()


public:
	bool bCharacterAbilitiesGiven = false;
	bool bStartupEffectsApplied = false;

	FReceivedDamageDelegate ReceivedDamageDelegate;

	// Called from DamageCalculation. Broadcasting on ReceiveDamage.
	virtual void ReceivedDamage(UCharacterAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage);
};
