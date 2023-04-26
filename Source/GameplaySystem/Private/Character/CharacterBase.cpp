// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterBase.h"

#include "Character/Abilities/CharacterAbilitySystemComponent.h"
#include "Character/Abilities/GGameplayAbility.h"
#include "Character/Abilities/AttributeSets/CharacterAttributeSetBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ACharacterBase::ACharacterBase(const FObjectInitializer& ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Collision Check
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Overlap);

	
	// Network Replication
	bAlwaysRelevant = true;

	// Corresponding GameplayTags to recurring Actions
	DeathTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	EffectRemoveOnDeathTag = FGameplayTag::RequestGameplayTag(FName("Effect.RemoveOnDeath"));
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
	
	// The name or Handle  of the Abilities, we want to remove
	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	
	// Iterating over all ActivatableAbilities
	for (const FGameplayAbilitySpec& Spec: AbilitySystemComponent->GetActivatableAbilities())
	{
		// Check if the Ability was created from this object(owner)
		//TODO: What exactly are we checking in the array?
		
		if ((Spec.SourceObject == this) && CharacterAbilities.Contains(Spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	// Second Time, only remove when we have to full list of Abilities
	for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}

	// Then tell the ASC that we do not have any Abilities left over
	AbilitySystemComponent->bCharacterAbilitiesGiven = false;
}

// Runs on the Server and all Clients
void ACharacterBase::Death()
{
	RemoveCharacterAbilities();

	// Take care of the  Character Visuals and Collision
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector(0);

	// Send Event, that we are dead
	// TODO: Who gets the Broadcast?
	OnCharacterDied.Broadcast(this);

	// Always check if we have an ASC
	if(AbilitySystemComponent.IsValid())
	{
		// The ASC takes care of Abilities
		AbilitySystemComponent->CancelAbilities();

		// Container that holds GameplayTags
		FGameplayTagContainer EffectTagsToRemove;
		// Fill Container with Tags
		EffectTagsToRemove.AddTag(EffectRemoveOnDeathTag);
		// Count the Number of removed Effects
		int32 NumOfEffectsRemoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectTagsToRemove);

		// Add the new tag, so we can react to the new state
		AbilitySystemComponent->AddLooseGameplayTag(DeathTag);
	}

	if(DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}
	else
	{
		FinishDeath();
	}
}

void ACharacterBase::FinishDeath()
{
	Destroy();
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

void ACharacterBase::AddCharacterAbilities()
{
	// The ASC and Server will grant us, if we don't have them already
	if(GetLocalRole() != ROLE_Authority
		|| false == AbilitySystemComponent.IsValid()
		|| AbilitySystemComponent->bCharacterAbilitiesGiven)
	{
		return;
	}
	
	// We check all Abilities inside our CharacterAbilities
	for (TSubclassOf<UGGameplayAbility>& Ability : CharacterAbilities)
	{
		// Because the ASC is the Root. It will start giving or removing Abilities
		AbilitySystemComponent->GiveAbility(
			// The Spec holds the data for each Ability
			FGameplayAbilitySpec(Ability,
				GetAbilityLevel(Ability.GetDefaultObject()->AbilityID)
				,static_cast<int32>(Ability.GetDefaultObject()->AbilityID)
				,this
			)
		);
	}
	
	// by now we should have granted the default abilities
	AbilitySystemComponent->bCharacterAbilitiesGiven = true;
}

void ACharacterBase::InitAttributes()
{
	if(false == AbilitySystemComponent.IsValid())
	{
		return;
	}
	
}

void ACharacterBase::AddStartupEffects()
{
	return;
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

