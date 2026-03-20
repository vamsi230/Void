// Fill out your copyright notice in the Description page of Project Settings.


#include "VHStaminaComponent.h"


// Sets default values for this component's properties
UVHStaminaComponent::UVHStaminaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UVHStaminaComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentStamina = MaxStamina;
	
	GetWorld()->GetTimerManager().SetTimer(StaminaTimerHandle, this, &UVHStaminaComponent::RegenStamina, TickRateCooldown, true);
	
}

bool UVHStaminaComponent::StaminaCost(float cost)
{
	if (CurrentStamina >= cost)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UVHStaminaComponent::RestoreRegen()
{
	bIsStaminaConsumed = false;
}

void UVHStaminaComponent::ApplyStaminaCost(float cost)
{
	bIsStaminaConsumed = true;
	CurrentStamina -= cost;
	CurrentStamina = FMath::Clamp(CurrentStamina, 0, MaxStamina);
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
	GetWorld()->GetTimerManager().SetTimer(RegenTimerHandle, this, &UVHStaminaComponent::RestoreRegen, RegenDelay, false);
}

bool UVHStaminaComponent::DrainStamina(float DrainRate)
{
	if (CurrentStamina < DrainRate) return false;
	CurrentStamina -= DrainRate;
	CurrentStamina = FMath::Clamp(CurrentStamina, 0, MaxStamina);
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
	return true;
}


void UVHStaminaComponent::RegenStamina()
{
	if (bIsStaminaConsumed) return;
	if (CurrentStamina == MaxStamina) return;
	CurrentStamina += RegenRate;
	CurrentStamina = FMath::Clamp(CurrentStamina, 0, MaxStamina);
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
}

