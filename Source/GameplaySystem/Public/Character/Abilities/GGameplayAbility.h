// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplaySystem/GameplaySystem.h"
#include "GGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYSYSTEM_API UGGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGGameplayAbility();

	// Automatically activate with this set of Input
	UPROPERTY(BlueprintReadOnly, EditAnywhere ,Category ="GAS|Abilites")
	GameplayAbilityID AbilityInputID = GameplayAbilityID::None;

	// Not tied to any slot. Passive abilities are not activated via input
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="GAS|Abilities")
	GameplayAbilityID AbilityID = GameplayAbilityID::None;

	// Tell the Ability to be activated when granted
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category ="GAS|Abilities")
	bool bActivateAbilityOnGranted = false;

	// If an ability is marked as 'ActivateAbilityOnGranted', activate them immediately when given here
	// Epic's comment: Projects may want to initiate passives or do other "BeginPlay" type of logic here.
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;	
};
