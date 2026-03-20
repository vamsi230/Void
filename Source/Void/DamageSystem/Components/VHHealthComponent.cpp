

#include "VHHealthComponent.h"


UVHHealthComponent::UVHHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CurrentHealth = 100.f;
	MaxHealth = 100.f;
}


void UVHHealthComponent::TakeDamage(float DamageAmount)
{
	if (bIsDead) return;

	CurrentHealth -= DamageAmount;	
	CurrentHealth =  FMath::Clamp(CurrentHealth, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
	if (CurrentHealth <= 0.f)
	{
		bIsDead = true;
		OnDeath.Broadcast();
	}
}

void UVHHealthComponent::Heal(float HealAmount)
{
	CurrentHealth += HealAmount;
	CurrentHealth =  FMath::Clamp(CurrentHealth, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}
