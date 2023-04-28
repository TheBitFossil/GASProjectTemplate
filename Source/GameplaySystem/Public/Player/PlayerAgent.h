// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Character/CharacterBase.h"
#include "PlayerAgent.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYSYSTEM_API APlayerAgent : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Setup theC-Tor with extra functionality. ObjInit is a helper for that
	APlayerAgent(const class FObjectInitializer& ObjectInitializer);

	// Calling from our PlayerController
	virtual void PossessedBy(AController* NewController) override;

	// Called from our CharacterBase 
	virtual void FinishDeath() override;

	/**	Camera Setup **/
	// Returns CameraBoom subobject
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	// Returns FollowCamera subobject
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable, Category="GAS|Camera")
	float GetStartingBoomLength();

	UFUNCTION(BlueprintCallable, Category="GAS|Camera")
	FVector GetStartingBoomLocation();
	
protected:
	// We want to bind Input, to actually move our player
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Gets called when the GAme starts or on spawn
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	/** Mouse Control **/
	void LookUp(float Value);
	void Turn(float Value);

	/** Mouse and Gamepad **/
	void MoveForward(float Value);
	void MoveRight(float Value);

	/** Gamepad **/
	void LookUpRate(float Value);
	void TurnRate(float Value);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="GAS|Camera")
	float BaseTurnRate = 45.0f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="GAS|Camera")
	float BaseLookUpRate = 45.0f;
	
	UPROPERTY(BlueprintReadOnly, Category="GAS|Camera")
	float StartingBoomLength;

	UPROPERTY(BlueprintReadOnly, Category="GAS|Camera")
	FVector StartingBoomLocation;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="GAS|Camera")
	class USpringArmComponent* CameraBoom;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere,Category="GAS|Camera")
	class UCameraComponent* FollowCamera;

	/** New InputSystem**/
	
	/*// Default Mapping Context for new InputSystem
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GAS|Input")
	class UInputMappingContext* DefaultInputMappingContext;

	// Move Action Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GAS|Input")
	class UInputAction* MoveAction;

	// Look Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GAS|Input")
	class UInputAction* LookAction;

	void MoveInput(const FInputActionValue& Value);
	void LookInput(const FInputActionValue& Value);*/

	
	/**	ASC **/
	bool ASCInputBound = false;
	FGameplayTag DeadTag;

	// Client Only
	virtual void OnRep_PlayerState() override;

	// Called from both SetupPlayerInputComponent and OnRep_PlayerState because of a potential race condition where the PlayerController might
	// call ClientRestart which calls SetupPlayerInputComponent before the PlayerState is repped to the client so the PlayerState would be null in SetupPlayerInputComponent.
	// Conversely, the PlayerState might be repped before the PlayerController calls ClientRestart so the Actor's InputComponent would be null in OnRep_PlayerState.
	void BindASCInput();
};
