// Fill out your copyright notice in the Description page of Project Settings.


#include "VHPlayerController.h"

#include "Blueprint/UserWidget.h"

void AVHPlayerController::BeginPlay()
{
	Super::BeginPlay();

	HUD = CreateWidget(this, HUDClass);

	if (HUD)
	{
		HUD->AddToViewport();
	}
}