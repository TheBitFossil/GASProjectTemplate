// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYSYSTEM_API AGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void CreateHUD();

	class UGUserWidget* GetHUD();

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="GAS|UI")
	TSubclassOf<class UGUserWidget> HUDWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category="GAS|UI")
	class UGUserWidget* HUDWidget;

	// Server only
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnRep_PlayerState() override;
};
