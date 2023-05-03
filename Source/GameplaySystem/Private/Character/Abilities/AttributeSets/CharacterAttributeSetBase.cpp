// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/AttributeSets/CharacterAttributeSetBase.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "Character/GCharacterBase.h"
#include "Character/Abilities/CharacterAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"


UCharacterAttributeSetBase::UCharacterAttributeSetBase()
{
}

// If we ADD a new Attribute, we also have to ADD it here
void UCharacterAttributeSetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// We notify when the values are changed
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSetBase, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSetBase, MaxHealth, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSetBase, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSetBase, MaxMana, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSetBase, CharacterLevel, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSetBase, MoveSpeed, COND_None, REPNOTIFY_Always);
	
}


void UCharacterAttributeSetBase::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSetBase, Health, OldHealth);
}

void UCharacterAttributeSetBase::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSetBase, MaxHealth, OldMaxHealth);
}

void UCharacterAttributeSetBase::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSetBase, Mana, OldMana);
}

void UCharacterAttributeSetBase::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSetBase, MaxMana, OldMaxMana);
}

void UCharacterAttributeSetBase::OnRep_CharacterLevel(const FGameplayAttributeData& OldCharacterLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSetBase, CharacterLevel, OldCharacterLevel);
}

void UCharacterAttributeSetBase::OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSetBase, MoveSpeed, OldMoveSpeed);
}

