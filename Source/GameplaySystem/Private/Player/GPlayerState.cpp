// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GPlayerState.h"
#include "Character/Abilities/CharacterAbilitySystemComponent.h"
#include "Character/Abilities/AttributeSets/CharacterAttributeSetBase.h"
#include "Player/GPlayerController.h"
#include "UI/GUserWidget.h"


AGPlayerState::AGPlayerState()
{
	// Add the AbilitySystemComponent so the Player can use the GAS
	AbilitySystemComponent = CreateDefaultSubobject<UCharacterAbilitySystemComponent>(TEXT("ASC"));
	// We need Network Replication
	AbilitySystemComponent->SetIsReplicated(true);
	//TODO: Read again about the different modes for Replication
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// Creation of the AttributeSet, Replication by default
	// Because we are adding it as subobject to OWNER it gets automatically registered
	AttributeSetBase = CreateDefaultSubobject<UCharacterAttributeSetBase>(TEXT("AttributeSet"));

	//PlayerState and Character share the same NetFrequency
	// Default is too low, it introduces lag: 100 is too high for shipping
	NetUpdateFrequency = 100.0f;

	// Cache Tags
	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

UAbilitySystemComponent* AGPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UCharacterAttributeSetBase* AGPlayerState::GetAttributeSet()
{
	return AttributeSetBase;
}


bool AGPlayerState::IsAlive() const
{
	// We get this from the AttributeSet
	// TODO: Why do we add this twice, once in the base character and second here?
	return GetHealth() > 0.0f;
}

void AGPlayerState::ShowAbilityConfirmCancelText(bool Message)
{
	AGPlayerController* PlayerController = Cast<AGPlayerController>(GetOwner());
	if(PlayerController)
	{
		UGUserWidget* HUD = PlayerController->GetHUD();
		if(HUD)
		{
			HUD->ShowAbilityConfirmCancelText(Message);
		}
	}
}

float AGPlayerState::GetHealth() const
{
	// The Health Attribute is on the AttributeSet
	// The value should be known by  the ASC
	return AttributeSetBase->GetHealth();
}

float AGPlayerState::GetMaxHealth() const
{
	return AttributeSetBase->GetMaxHealth();
}

float AGPlayerState::GetMana() const
{
	return  AttributeSetBase->GetMana();
}

float AGPlayerState::GetMaxMana() const
{
	return AttributeSetBase->GetMaxMana();
}

int32 AGPlayerState::GetCharacterLevel() const
{
	return AttributeSetBase->GetCharacterLevel();
}

void AGPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if(AbilitySystemComponent)
	{
		// Fill out the Data from out Attributes
		HealthChangedDelegateHandle		= AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &AGPlayerState::HealthChanged);
		ManaChangedDelegateHandle		= AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetManaAttribute()).AddUObject(this, &AGPlayerState::ManaChanged);
		
		MaxHealthChangedDelegateHandle	= AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxHealthAttribute()).AddUObject(this, &AGPlayerState::MaxHealthChanged);
		MaxManaChangedDelegateHandle	= AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxHealthAttribute()).AddUObject(this, &AGPlayerState::MaxManaChanged);

		CharacterLevelDelegateHandle	= AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetCharacterLevelAttribute()).AddUObject(this, &AGPlayerState::CharacterLevelChanged);
	}
}

void AGPlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
	//float Health = Data.NewValue;

	// Update floating StatusBar
	return;
}

void AGPlayerState::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	return;

}

void AGPlayerState::ManaChanged(const FOnAttributeChangeData& Data)
{
	return;

}

void AGPlayerState::MaxManaChanged(const FOnAttributeChangeData& Data)
{	return;

}

void AGPlayerState::CharacterLevelChanged(const FOnAttributeChangeData& Data)
{	return;

}
