// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/AttributeSets/CharacterAttributeSetBase.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "Character/GCharacterBase.h"
#include "Character/Abilities/CharacterAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"


UCharacterAttributeSetBase::UCharacterAttributeSetBase()
{
}

// If we ADD a new Attribute, we also have to ADD it here
void UCharacterAttributeSetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// We notify when the values are changed
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSetBase, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSetBase, MaxHealth, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSetBase, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSetBase, MaxMana, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSetBase, CharacterLevel, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSetBase, MoveSpeed, COND_None, REPNOTIFY_Always);
	
}

void UCharacterAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// The macros from the header data give us this function call
	if(Attribute == GetMoveSpeedAttribute())
	{
		// Just clamping between arbitrary number: this is an example
		NewValue = FMath::Clamp<float>(NewValue, 150, 1000);
	}
}

void UCharacterAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	/** Basic Setup for all Abilities **/
	
	// We are creating Context to store information about the Ability
	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	// The AbilityContext knows about the owning AbilitySystemComponent
	UAbilitySystemComponent* ASC = Context.GetOriginalInstigatorAbilitySystemComponent();
	// We are working with the Reference, so we need to dereference the pointer, to get its data content
	const FGameplayTagContainer& ASCTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

	// Local variable for Tags of our Ability
	FGameplayTagContainer SpecAssetTags;
	// Then we are getting them into the Variable
	Data.EffectSpec.GetAllAssetTags(SpecAssetTags);

	/** Target Actor Data
	 *	The Actor who has the ASC attached
	 * **/
	AActor* OwnerActor = nullptr;
	AController* OwnerController = nullptr;
	ACharacterBase* OwnerCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		OwnerActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		OwnerController = Data.Target.AbilityActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacterBase>(OwnerActor);
	}
	
	/** Source Actor Data
	 *	The physical representation of the ASC Actor
	 *	Can be the same as the Owner, in a MOBA it would be best to use the PlayerState
	 *	so the stats remain after respawn
	 * **/
	AActor* SourceActor = nullptr;
	AController* SourceController = nullptr;
	ACharacterBase* SourceCharacter = nullptr;
	if(ASC && ASC->AbilityActorInfo.IsValid() && ASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		SourceActor = ASC->AbilityActorInfo->AvatarActor.Get();
		SourceController = ASC->AbilityActorInfo->PlayerController.Get();
		// Different ways to get the Actor and the Controller from either one
		// Depending on who we find from the pointer
		if(SourceController)
		{
			SourceCharacter = Cast<ACharacterBase>(SourceController->GetPawn());
		}
		else
		{
			SourceCharacter = Cast<ACharacterBase>(SourceActor);
		}

		if(SourceController == nullptr && SourceActor != nullptr)
		{
			if(APawn* Pawn = Cast<APawn>(SourceActor))
			{
				SourceController = Pawn->GetController();
			}
		}

		// Actor who is the "Causer"
		if(Context.GetEffectCauser())
		{
			SourceActor = Context.GetEffectCauser();
		}
	}


	// DeltaValue if we add values to our existing
	float DeltaValue = 0.f;
	if(Data.EvaluatedData.ModifierOp == EGameplayModOp::Additive)
	{
		// If this was additive, store the Raw Delta Value, which we pass later
		DeltaValue = Data.EvaluatedData.Magnitude;
	}
	/** End of Basic Setup**/

	/** Working with the Ability Data (struct)**/
	if(Data.EvaluatedData.Attribute == GetMoveSpeedAttribute())
	{
		if(SourceCharacter)
		{
			SourceCharacter->HandleMoveSpeedChange(DeltaValue, ASCTags);
		}
	}
	
	
}

void UCharacterAttributeSetBase::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSetBase, Health, OldHealth);
}

void UCharacterAttributeSetBase::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSetBase, MaxHealth, OldMaxHealth);
}

void UCharacterAttributeSetBase::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSetBase, Mana, OldMana);
}

void UCharacterAttributeSetBase::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSetBase, MaxMana, OldMaxMana);
}

void UCharacterAttributeSetBase::OnRep_CharacterLevel(const FGameplayAttributeData& OldCharacterLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSetBase, CharacterLevel, OldCharacterLevel);
}

void UCharacterAttributeSetBase::OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSetBase, MoveSpeed, OldMoveSpeed);
}

