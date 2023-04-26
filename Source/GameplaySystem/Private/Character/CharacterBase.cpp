// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterBase.h"

#include "Character/Abilities/CharacterAbilitySystemComponent.h"
#include "Character/Abilities/AttributeSets/CharacterAttributeSetBase.h"

// Sets default values
ACharacterBase::ACharacterBase(const FObjectInitializer& ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Network Replication
	bAlwaysRelevant = true;

	// Corresponding GameplayTags to recurring Actions
	DeathTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	EffectRemovedTag = FGameplayTag::RequestGameplayTag(FName("Effect.RemoveOnDeath"));
}

bool ACharacterBase::IsAlive() const
{
	return GetHealth() > 0.0f;
}

int32 ACharacterBase::GetAbilityLevel(GameplayAbilityID AbilityID) const
{
	return 1;
}

void ACharacterBase::RemoveCharacterAbilities()
{
	// We are removing the Abilities from server side
	// Lets make sure that we are on the Server, Have AbilitySystem, Have granted Abilities
	if(GetLocalRole() != ROLE_Authority
		|| false == AbilitySystemComponent.IsValid()
		|| false == AbilitySystemComponent->bCharacterAbilitiesGiven)
	{
		return;
	}
	
	// Remove any Abilities from a granted call
	TArray<FGameplayAbilitySpec> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec: AbilitySystemComponent->GetActivatableAbilities())
	{
		// Check if the Ability was created from this object(owner) and CharacterAbilities has 
		//if(Spec.SourceObject == this && CharacterAbilities)

		
		AbilitiesToRemove.Add(Spec);
	}
}

void ACharacterBase::Death()
{
}

void ACharacterBase::FinishDeath()
{
}

float ACharacterBase::GetHealth() const
{
	// Health value is stored inside the AttributeSetBase
	if(AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetHealth();
	}
	// If the AttributeSet is not initialized, we don't have Values to work with
	return 0.0f;
}

float ACharacterBase::GetMaxHealth() const
{
	if(AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMaxHealth();
	}
	return 0.0f;
}

float ACharacterBase::GetMana() const
{
	if(AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMana();
	}
	return 0.0f;
}

float ACharacterBase::GetMaxMana() const
{
	if(AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMaxMana();
	}
	return 0.0f;
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACharacterBase::SetHealth(float NewHealth)
{
	if(AttributeSetBase.IsValid())
	{
		AttributeSetBase->SetHealth(NewHealth);
	}
}

void ACharacterBase::SetMana(float NewMana)
{
	if(AttributeSetBase.IsValid())
	{
		AttributeSetBase->SetMana(NewMana);
	}
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

