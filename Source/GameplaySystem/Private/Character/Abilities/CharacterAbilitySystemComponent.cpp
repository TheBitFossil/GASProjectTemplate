// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/CharacterAbilitySystemComponent.h"

void UCharacterAbilitySystemComponent::ReceivedDamage(UCharacterAbilitySystemComponent* SourceASC,
	float UnmitigatedDamage, float MitigatedDamage)
{
	ReceivedDamageDelegate.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
}
