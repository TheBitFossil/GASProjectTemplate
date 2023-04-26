// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameplaySystem/GameplaySystem.h"
#include "CharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, ACharacterBase*, Character);


UCLASS()
class GAMEPLAYSYSTEM_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase(const class FObjectInitializer& ObjectInitializer);

	// Death Event Delegate
	UPROPERTY(BlueprintAssignable, Category ="GAS|Character")
	FCharacterDiedDelegate OnCharacterDied;

	UFUNCTION(BlueprintCallable, Category ="GAS|Character")
	virtual bool IsAlive() const;

	// Return the individual ability we are using right now
	UFUNCTION(BlueprintCallable, Category ="GAS|Character")
	virtual int32 GetAbilityLevel(GameplayAbilityID AbilityID) const;

	// Remove the individual ability outgoing from the Server
	virtual void RemoveCharacterAbilities();

	// Death handling
	virtual void Death();
	
	UFUNCTION(BlueprintCallable, Category ="GAS|Character")
	virtual void FinishDeath();

	// Getters for our AttributeSet
	UFUNCTION(BlueprintCallable, Category="GAS|Character|Attributes")
	float GetHealth() const;
	
	UFUNCTION(BlueprintCallable, Category="GAS|Character|Attributes")
	float GetMaxHealth() const;
	
	UFUNCTION(BlueprintCallable, Category="GAS|Character|Attributes")
	float GetMana() const;
	
	UFUNCTION(BlueprintCallable, Category="GAS|Character|Attributes")
	float GetMaxMana() const;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Interface Object needs to be created before we can make a call to it
	// But we are using a soft reference for it
	TWeakObjectPtr<class UCharacterAbilitySystemComponent> AbilitySystemComponent;
	// Same for the AttributeSet
	TWeakObjectPtr<class UCharacterAttributeSetBase> AttributeSetBase;

	// Gameplay Tags
	FGameplayTag DeathTag;
	FGameplayTag EffectRemovedTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category ="GAS|Anims")
	UAnimMontage* DeathMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category ="GAS|Character")
	FText CharacterName;

	// This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category ="GAS|Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	// Default Abilities that are granted on respawn and taken on death
	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="GAS|Abilities")
//	TSubclassOf<class UAbili>

	
	// Setters for our AttributeSet
	virtual void SetHealth(float NewHealth);
	virtual void SetMana(float NewMana);

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
};
