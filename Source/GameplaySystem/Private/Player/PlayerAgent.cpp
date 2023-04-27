// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerAgent.h"

#include "AI/PlayerAIController.h"
#include "Camera/CameraComponent.h"
#include "Character/Abilities/CharacterAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/GPlayerState.h"

APlayerAgent::APlayerAgent(const FObjectInitializer& ObjectInitializer)
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(FName("Camera Boom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetRelativeLocation((FVector(0,0, 70)));

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(FName("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->SetFieldOfView(80);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	/** Animations play on the Server to use bone and socket transforms ( spawning projectiles and fx )	**/
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(FName("NoCollision"));

	/**	AI Controller Class	**/
	// TODO: Why should I Use AI Controller here
	AIControllerClass = APlayerAIController::StaticClass();

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

void APlayerAgent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerAgent::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerAgent::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerAgent::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerAgent::Turn);
	
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerAgent::LookUpRate);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerAgent::TurnRate);

	// Bind ASC and Player Input : OnRep_PlayerState
	BindASCInput();
}

void APlayerAgent::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AGPlayerState* PState = GetPlayerState<AGPlayerState>();
	if(PState)
	{
		// Get the ASC from our PlayerState
		AbilitySystemComponent = Cast<UCharacterAbilitySystemComponent>(PState->GetAbilitySystemComponent());
		// Init the ASC from our PlayerState
		PState->GetAbilitySystemComponent()->InitAbilityActorInfo(PState,this);

		// Get the AttributeSet from our PlayerState
		AttributeSetBase = PState->GetAttributeSet();

		// TODO: Check the dependencies between State, Base, Controller , etc and make a visual graph
		// Init from our CharacterBase
		InitAttributes();
		AddStartupEffects();
		AddCharacterAbilities();

		/** Respawning	**/

		/** End of Respawning **/

		/**	UI **/
		
	}
}

void APlayerAgent::FinishDeath()
{
	Super::FinishDeath();
	// TODO
}

USpringArmComponent* APlayerAgent::GetCameraBoom()
{
	return CameraBoom;
}

UCameraComponent* APlayerAgent::GetFollowCamera()
{
	return FollowCamera;
}

float APlayerAgent::GetStartingBoomLength()
{
	return StartingBoomLength;
}

FVector APlayerAgent::GetStartingBoomLocation()
{
	return StartingBoomLocation;
}

void APlayerAgent::BeginPlay()
{
	Super::BeginPlay();

	// Init Camera
	StartingBoomLength = CameraBoom->TargetArmLength;
	StartingBoomLocation = CameraBoom->GetRelativeLocation();
}

void APlayerAgent::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APlayerAgent::LookUp(float Value)
{
	if(IsAlive())
	{
		AddControllerPitchInput(Value);
	}
}

void APlayerAgent::Turn(float Value)
{
	if(IsAlive())
	{
		AddControllerYawInput(Value);
	}
}

void APlayerAgent::MoveForward(float Value)
{
	if(IsAlive())
	{
		AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
	}
}

void APlayerAgent::MoveRight(float Value)
{
	if(IsAlive())
	{
		AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
	}
}

void APlayerAgent::LookUpRate(float Value)
{
	if(IsAlive())
	{
		AddControllerPitchInput(Value * BaseLookUpRate * GetWorld()->DeltaTimeSeconds);
	}
}

void APlayerAgent::TurnRate(float Value)
{
	AddControllerYawInput(Value * BaseTurnRate * GetWorld()->DeltaTimeSeconds);
}

void APlayerAgent::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}

void APlayerAgent::BindASCInput()
{
	
}
