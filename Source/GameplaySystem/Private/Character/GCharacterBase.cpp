// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GCharacterBase.h"

#include "Character/GCharacterMovementComponent.h"
#include "Character/Abilities/CharacterAbilitySystemComponent.h"
#include "Character/Abilities/GGameplayAbility.h"
#include "Character/Abilities/AttributeSets/CharacterAttributeSetBase.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ACharacterBase::ACharacterBase(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UGCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
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

int32 ACharacterBase::GetCharacterLevel() const
{
	if(AbilitySystemComponent.IsValid())
	{
		// TODO: Why am I using static_cast so often?
		return static_cast<int32>(AttributeSetBase->GetCharacterLevel());
	}
	return 0;
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

/*void ACharacterBase::HandleMoveSpeedChange(float DeltaValue, FGameplayTagContainer AscTags)
{
	// Update Character Move & WalkSpeed
	GetCharacterMovement()->MaxWalkSpeed = GetMoveSpeed();

	// Call a blueprint implementable event
}*/

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

float ACharacterBase::GetMoveSpeed() const
{
	if(AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMoveSpeed();
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
	// Only on the Server: ASC is granting us Abilities, if we don't have them already
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

	if(nullptr == DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint.")
			,*FString(__FUNCTION__)
			,*GetName());

		return;
	}

	// Where the Effect will be added to: Can be run on Server and Client
	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	// What the Effect is made of: Data for our next Attributes
	FGameplayEffectSpecHandle NewHandle =
		AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes,GetCharacterLevel(),EffectContextHandle);

	// Do we have an Effect to add: Check if the Data is available to us, so we can Initialize it
	if(NewHandle.IsValid())
	{
		// We do have Attributes to initialize
		// TODO: Check why I need to dereference here
		FActiveGameplayEffectHandle ActiveGEHandle =
			AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(
				*NewHandle.Data.Get()
				,AbilitySystemComponent.Get());
	}
}

void ACharacterBase::AddStartupEffects()
{
	if(GetLocalRole() != ROLE_Authority
		|| false == AbilitySystemComponent.IsValid()
		|| AbilitySystemComponent->bStartupEffectsApplied)
	{
		return;
	}

	// Where we are using this Effect, the owner
	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	// Get all Effects that are available for Startup
	for (const TSubclassOf<UGameplayEffect>& Effect: StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle =
			AbilitySystemComponent->MakeOutgoingSpec(Effect, GetCharacterLevel(), EffectContextHandle);

		// If we found an Effect
		if(NewHandle.IsValid())
		{
			// We can then apply it to the owner, via the ASC
			FActiveGameplayEffectHandle ActiveGEHandle =
				AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(
					*NewHandle.Data.Get()
					,AbilitySystemComponent.Get()
				);
		}
	}

	// Set the flag, that we have added Startup Effects
	AbilitySystemComponent->bStartupEffectsApplied = true;
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

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

