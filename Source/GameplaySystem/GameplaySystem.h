// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class GameplayAbilityID: uint8
{
	None UMETA(DisplayName = "None"),
	Confirm UMETA(Displayname = "Confirm"),
	Cancel UMETA(DisplayName = "Cancel"),
	
	// Movement Skill: Dash, Roll
	Ability1 UMETA(DisplayName ="Ability1"),

	// Defensive Skill: Roll, OakMode
	Ability2 UMETA(DisplayName ="Ability2"),
	
	// Offensive Skill: LockOn, Invisible 
	Ultimate UMETA(DisplayName ="Ultimate"),

	// Attacking: Shoot, Melee
	Ability3 UMETA(DisplayName = "Ability3"),
	
	// Aiming: Weapon Scope
	Ability4 UMETA(DisplayName = "Ability4"),
};
