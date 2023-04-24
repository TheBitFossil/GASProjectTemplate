// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "GAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYSYSTEM_API UGAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UGAssetManager& Get();
	
	// We have to setup our ASC
	virtual void StartInitialLoading() override;
	
};
