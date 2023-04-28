// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerAgent.h"

#include "AI/PlayerAIController.h"
#include "Camera/CameraComponent.h"
#include "Character/Abilities/AttributeSets/CharacterAttributeSetBase.h"
#include "Character/Abilities/CharacterAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/GPlayerController.h"
#include "Player/GPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"

APlayerAgent::APlayerAgent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	/** Camera Setup **/
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(FName("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SetRelativeLocation((FVector(0,0,75)));
	StartingBoomLength = CameraBoom->TargetArmLength;
	StartingBoomLocation = CameraBoom->GetRelativeLocation();
	// Does the Boom rotate relative to the Controller ?
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(FName("FollowCamera"));
	// Attach the Camera to the end of the Camera Boom
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->SetFieldOfView(85);
	// Does the Camera rotate relative to Boom ?
	FollowCamera->bUsePawnControlRotation = false;


	/** Capsule Collision **/
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->InitCapsuleSize(55.f, 100.f);

	/*// Rotation only for the Camera
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;*/

	/** Character Movement **/
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.0f, 0.f);
	
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

	/**	New Inputsystem **/
	/*
	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerAgent::MoveInput);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerAgent::LookInput);
	}
	*/

	
	// Bind ASC and Player Input : OnRep_PlayerState
	BindASCInput();
}

void APlayerAgent::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AGPlayerState* PState = GetPlayerState<AGPlayerState>();
	if(PState)
	{
		// Set the ASC on the Server. Client does this from OnRep_PlayerState()
		AbilitySystemComponent = Cast<UCharacterAbilitySystemComponent>(PState->GetAbilitySystemComponent());

		// Init the ASC from our PlayerState. For the AI
		PState->GetAbilitySystemComponent()->InitAbilityActorInfo(PState,this);

		// Get the AttributeSet from our PlayerState
		AttributeSetBase = PState->GetAttributeSetBase();
		
		// TODO: Check the dependencies between State, Base, Controller , etc and make a visual graph
		// Init from our CharacterBase
		InitAttributes();

		// Set DeadTag to 0.
		AbilitySystemComponent->SetTagMapCount(DeadTag, 0);
		
		/** Respawning	**/
		SetHealth(GetMaxHealth());
		SetMana(GetMaxMana());
		/** End of Respawning **/

		AddStartupEffects();
		AddCharacterAbilities();
		
		/**	UI **/
		//TODO: UI
	}
}

void APlayerAgent::FinishDeath()
{
	Super::FinishDeath();
	// TODO: Finish Death
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

	/*// Init the Input Mapping Context
	if(AGPlayerController* PlayerController = Cast<AGPlayerController>(Controller))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem
			= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultInputMappingContext, 0);
		}
	}*/
	

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
	const FString msg = FString::Printf(TEXT("Bool Status: %f"), GetHealth());
	GEngine->AddOnScreenDebugMessage(-1, .5f, FColor::Red, *msg, true);

	if(IsAlive())
	{
		AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
	}
	if(!IsAlive())
	{
		UE_LOG(LogTemp, Warning, TEXT("We are dead!"));
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
	if(IsAlive())
	{
		AddControllerYawInput(Value * BaseTurnRate * GetWorld()->DeltaTimeSeconds);
	}
}

/*void APlayerAgent::MoveInput(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if(Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Forward Vector
		const FVector FwdVec = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// Right Vector
		const FVector RVec = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(FwdVec *MovementVector.Y);
		AddMovementInput(RVec *MovementVector.X);
	}
}

void APlayerAgent::LookInput(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if(Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}*/

// Client Only
void APlayerAgent::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AGPlayerState* PState = GetPlayerState<AGPlayerState>();
	if(PState)
	{
		// Init ASC for Clients. Server does this in PossessedBy()
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
