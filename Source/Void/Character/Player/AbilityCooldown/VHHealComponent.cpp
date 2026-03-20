// Fill out your copyright notice in the Description page of Project Settings.


#include "VHHealComponent.h"

#include "Void/Character/Player/VHPlayerCharacter.h"
#include "Void/DamageSystem/Components/VHHealthComponent.h"


// Sets default values for this component's properties
UVHHealComponent::UVHHealComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UVHHealComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UVHHealComponent::ActivateAbility()
{
	if (bIsOnCooldown) return;
	AVHPlayerCharacter* PlayerCharacter = Cast<AVHPlayerCharacter>(GetOwner());
	if (!PlayerCharacter) return;
	if (PlayerCharacter->HealthComponent->CurrentHealth == PlayerCharacter->HealthComponent->MaxHealth) return;
	Super::ActivateAbility();
	
	UVHHealthComponent* HealthComponent = GetOwner()->FindComponentByClass<UVHHealthComponent>();

	if (HealthComponent)
	{
		HealthComponent->Heal(HealAmount);
	}
}

