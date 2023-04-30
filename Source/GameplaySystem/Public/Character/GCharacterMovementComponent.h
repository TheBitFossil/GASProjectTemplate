// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYSYSTEM_API UGCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UGCharacterMovementComponent();

	// TODO: What does this declaration exactly mean?
	uint32 IsRequestingSprint : 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="GAS|Sprint")
	float SprintSpeedMulti;

	virtual float GetMaxSpeed()const override;

	UFUNCTION(BlueprintCallable, Category ="GAS|Sprint")
	void StartToSprint();

	UFUNCTION(BlueprintCallable, Category ="GAS|Sprint")
	void EndOfSprint();
};
