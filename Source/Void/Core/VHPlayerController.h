// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VHPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class VOID_API AVHPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> HUDClass;

	UPROPERTY(EditDefaultsOnly)
	UUserWidget* HUD;
	
	
	virtual void BeginPlay() override;
};
