// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class GameplayAbilityID: uint8
{
	None UMETA(DisplayName = "None"),
	Confirm UMETA(Displayname = "Confirm"),
	Cancel UMETA(DisplayName = "Cancel"),
	Ability1 UMETA(DisplayName ="Ability1")
};
