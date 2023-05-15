// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CharacterAttributeSetBase.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class GAMEPLAYSYSTEM_API UCharacterAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()


public:
	UCharacterAttributeSetBase();

	//Returns properties used by Network Replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Overrides to handle PreAbility and PostAbility options
	// We could clamp values here, or do extra damage calculations
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, Health)
	
	UPROPERTY(BlueprintReadOnly, Category="Health", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category="Mana", ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, Mana);

	UPROPERTY(BlueprintReadOnly, Category="Mana", ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, MaxMana);

	UPROPERTY(BlueprintReadOnly, Category="Character Level", ReplicatedUsing = OnRep_CharacterLevel)
	FGameplayAttributeData CharacterLevel;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, CharacterLevel);

	UPROPERTY(BlueprintReadOnly, Category="Stamina", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, Stamina);

	UPROPERTY(BlueprintReadOnly, Category="Stamina", ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, MaxStamina);

	UPROPERTY(BlueprintReadOnly, Category="Stamina", ReplicatedUsing = OnRep_StaminaRegen)
	FGameplayAttributeData StaminaRegen;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, StaminaRegen);
	
	// Cached Damage value, to better calculate total damage from all possible AbilitySources
	// Only exist on the server
	UPROPERTY(BlueprintReadOnly, Category ="Damage")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, Damage);
	
	/**	Sprint Ability **/
	UPROPERTY(BlueprintReadOnly, Category ="Movement", ReplicatedUsing =OnRep_MoveSpeed)
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase,MoveSpeed);

	
protected:
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_Mana(const FGameplayAttributeData& OldMana);

	UFUNCTION()
	virtual void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana);

	UFUNCTION()
	virtual void OnRep_CharacterLevel(const FGameplayAttributeData& OldCharacterLevel);

	UFUNCTION()
	virtual void OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed);

	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);
	
	UFUNCTION()
	virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);

	UFUNCTION()
	virtual void OnRep_StaminaRegen(const  FGameplayAttributeData& OldStaminaRegen);
};
