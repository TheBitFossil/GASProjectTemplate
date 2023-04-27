// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerAgent.h"

#include "AI/PlayerAIController.h"
#include "Camera/CameraComponent.h"
#include "Character/Abilities/CharacterAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/GPlayerController.h"
#include "Player/GPlayerState.h"

APlayerAgent::APlayerAgent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
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
		AttributeSetBase = PState->GetAttributeSetBase();
		
		// TODO: Check the dependencies between State, Base, Controller , etc and make a visual graph
		// Init from our CharacterBase
		InitAttributes();
		AddStartupEffects();
		AddCharacterAbilities();

		/** Respawning	**/

		/** End of Respawning **/

		/**	UI **/
		//TODO: UI
		
	}
}

void APlayerAgent::FinishDeath()
{
	Super::FinishDeath();
	// TODO: Finish Death
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

/** From Documentation:
* On the Server, Possession happens before BeginPlay.
* On the Client, BeginPlay happens before Possession.
* So we can't use BeginPlay to do anything with the AbilitySystemComponent because we don't have it until the PlayerState replicates from possession.
*/
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
	// TODO: Gun here
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

// Client Only
void APlayerAgent::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AGPlayerState* PState = GetPlayerState<AGPlayerState>();
	if(PState)
	{
		// Init ASC for Clients. Server does this in PossessedBy
		AbilitySystemComponent = Cast<UCharacterAbilitySystemComponent>(PState->GetAbilitySystemComponent());

		// Init ASC Actor Infor for CLients. Server will init in Possess for a new Actor
		AbilitySystemComponent->InitAbilityActorInfo(PState, this);

		// Input Binds also called SetupPlayerInputComponent. Race Condition safety
		BindASCInput();

		// AttributeSetBase
		AttributeSetBase = PState->GetAttributeSetBase();

		// TODO change this is respawning should not reset all Attributes
		InitAttributes();

		/*// We can get the PlayerController from internal Function. Only need to Cast to our Type
		AGPlayerController* PController = Cast<AGPlayerController>(GetController());
		if(PController)
		{
			TODO: CreateHUD();
		}*/


		// GameplayTags
		AbilitySystemComponent->SetTagMapCount(DeadTag,0);

		// On Respawn, we are initializing our Values to Max
		SetHealth(GetMaxHealth());
		SetMana(GetMaxMana());
	}
}

void APlayerAgent::BindASCInput()
{
	// Template to bind input and ASC from Documentation
	if(false == ASCInputBound && AbilitySystemComponent.IsValid() && IsValid(InputComponent))
	{
		FTopLevelAssetPath AbilityEnumAssetPath = FTopLevelAssetPath(FName("/Script/GameplaySystem"), FName("GameplayAbilityID"));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent
			,FGameplayAbilityInputBinds
			(
				FString("ConfirmTarget")
				,FString("CancelTarget")
				,AbilityEnumAssetPath
				,static_cast<int32>(GameplayAbilityID::Confirm)
				,static_cast<int32>(GameplayAbilityID::Cancel)
			)
		);

		// Check flag that the input is now ready
		ASCInputBound = true;
	}
}
