// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <elf.h>

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerState.h"
#include "GPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYSYSTEM_API AGPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AGPlayerState();

	// Implemented by the IAbilityInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	class UGAttributeSetBase* GetAttributeSet();

	UFUNCTION(BlueprintCallable, Category="GAS|PlayerState")
	bool IsAlive() const;

	/**	Getters and Setters	from the AttributeSetBase ( Current Value )**/
	UFUNCTION(BlueprintCallable, Category="GAS|PlayerState|Attributes")
	float GetHealth() const;
	
	UFUNCTION(BlueprintCallable, Category="GAS|PlayerState|Attributes")
	float GetMaxHealth() const;
	
	UFUNCTION(BlueprintCallable, Category="GAS|PlayerState|Attributes")
	float GetMana() const;
	
	UFUNCTION(BlueprintCallable, Category="GAS|PlayerState|Attributes")
	float GetMaxMana() const;
	
	UFUNCTION(BlueprintCallable, Category="GAS|PlayerState|Attributes")
	int32 GetCharacterLevel() const;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY()
	class UGAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	class UGAttributeSetBase* AttributeSetBase;

	FGameplayTag DeadTag;

	/** Event Callbacks  **/
	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;
	
	FDelegateHandle ManaChangedDelegateHandle;
	FDelegateHandle MaxManaChangedDelegateHandle;
	
	FDelegateHandle CharacterLevelDelegateHandle;

	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void ManaChanged(const FOnAttributeChangeData& Data);
	virtual void MaxManaChanged(const FOnAttributeChangeData& Data);
	virtual void CharacterLevelChanged(const FOnAttributeChangeData& Data);

	




};
