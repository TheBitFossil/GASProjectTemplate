// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "Abilities/AttributeSets/CharacterAttributeSetBase.h"
#include "GameFramework/Character.h"
#include "GameplaySystem/GameplaySystem.h"
#include "GCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, ACharacterBase*, Character);


UCLASS()
class GAMEPLAYSYSTEM_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase(const class FObjectInitializer& ObjectInitializer);

	// Implement AbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
		
	UFUNCTION(BlueprintCallable, Category ="GAS|Character")
	virtual bool IsAlive() const;

	// Return the individual ability we are using right now
	UFUNCTION(BlueprintCallable, Category ="GAS|Character")
	virtual int32 GetAbilityLevel(GameplayAbilityID AbilityID) const;

	// Remove the individual ability outgoing from the Server
	virtual void RemoveCharacterAbilities();

	UFUNCTION(BlueprintCallable, Category ="GAS|Character")
	virtual int32 GetCharacterLevel() const;

	// Death handling
	virtual void Death();
	
	UFUNCTION(BlueprintCallable, Category ="GAS|Character")
	virtual void FinishDeath();

	// Death Event Delegate
	UPROPERTY(BlueprintAssignable, Category ="GAS|Character")
	FCharacterDiedDelegate OnCharacterDied;
	
	// Getters for our AttributeSet
	UFUNCTION(BlueprintCallable, Category="GAS|Character|Attributes")
	float GetHealth() const;
	
	UFUNCTION(BlueprintCallable, Category="GAS|Character|Attributes")
	float GetMaxHealth() const;
	
	UFUNCTION(BlueprintCallable, Category="GAS|Character|Attributes")
	float GetMana() const;
	
	UFUNCTION(BlueprintCallable, Category="GAS|Character|Attributes")
	float GetMaxMana() const;

	UFUNCTION(BlueprintCallable, Category="GAS|Character|Attributes")
	float GetMoveSpeed() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Interface Object needs to be created before we can make a call to it **/

	// using a soft reference for it. The real components are on the PlayerState
	TWeakObjectPtr<class UCharacterAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<class UCharacterAttributeSetBase> AttributeSetBase;
	
	// Gameplay Tags
	FGameplayTag DeathTag;
	FGameplayTag EffectRemoveOnDeathTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category ="GAS|Anims")
	UAnimMontage* DeathMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category ="GAS|Character")
	FText CharacterName;

	// This is an instant GE that overrides the values for Default Attributes
	// get reset on spawn/respawn.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category ="GAS|Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	// This is an instant GE that overrides the value for Default Abilities
	// regiven on respawn and removed on death
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="GAS|Abilities")
	TArray<TSubclassOf<class UGGameplayAbility>> CharacterAbilities;

	// These Effects only apply on startup
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="GAS|Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;
	
	// Server grants Abilities
	virtual void AddCharacterAbilities();

	// Server grants Attributes, can be run on client for faster init
	virtual void InitAttributes();

	// Here we are adding the Effects to our Character
	virtual void AddStartupEffects();

	// Setters for our AttributeSet
	virtual void SetHealth(float NewHealth);
	virtual void SetMana(float NewMana);
};
