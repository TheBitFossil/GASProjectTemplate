// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GCharacterMovementComponent.h"
#include "Character/GCharacterBase.h"

UGCharacterMovementComponent::UGCharacterMovementComponent()
{
	SprintSpeedMulti = 1.7f;
}

float UGCharacterMovementComponent::GetMaxSpeed() const
{
	ACharacterBase* Owner = Cast<ACharacterBase>(GetOwner());

	if(!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() No Owner!"), *FString(__FUNCTION__));
		// Unreal Engine CharacterMovementComp
		return Super::GetMaxSpeed();
	}

	// We stop moving if we are dead
	if(false == Owner->IsAlive())
	{
		return 0.0f;
	}

	// We have pressed the Input and through the BP we can call that we are sprinting
	if(IsRequestingSprint)
	{
		// MoveSpeed is an Attribute and the BaseCharacter has access to it
		return Owner->GetMoveSpeed() * SprintSpeedMulti;
	}
	
	return Owner->GetMoveSpeed();
}

void UGCharacterMovementComponent::StartToSprint()
{
	IsRequestingSprint = true;
}

void UGCharacterMovementComponent::EndOfSprint()
{
	IsRequestingSprint = false;
}
