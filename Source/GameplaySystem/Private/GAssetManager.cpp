// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplaySystem/Public/GAssetManager.h"
#include "AbilitySystemGlobals.h"


UGAssetManager& UGAssetManager::Get()
{
	UGAssetManager* Singleton = Cast<UGAssetManager>(GEngine->AssetManager);

	if(Singleton)
	{
		return *Singleton;
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManager in DefaultEngine.ini, must be GDAssetManager!"));
		return *NewObject<UGAssetManager>();
	}
	
}

void UGAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	UAbilitySystemGlobals::Get().InitGlobalData();
}
