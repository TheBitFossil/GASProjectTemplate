// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterBase.h"

#include "Character/Abilities/AttributeSets/CharacterAttributeSetBase.h"

// Sets default values
ACharacterBase::ACharacterBase(const FObjectInitializer& ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

bool ACharacterBase::IsAlive() const
{
}

int32 ACharacterBase::GetAbilityLevel(GameplayAbilityID AbilityID) const
{
}

void ACharacterBase::RemoveCharacterAbilities()
{
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
	return GetAbilitySystemComponent();
}

