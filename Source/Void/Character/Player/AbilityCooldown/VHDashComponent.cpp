#include "Void/Character/Player/AbilityCooldown/VHDashComponent.h"
#include "Void/Character/Player/VHPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Void/DamageSystem/Components/VHStaminaComponent.h"

UVHDashComponent::UVHDashComponent()
{
	SetIsReplicatedByDefault(false);
	DashStrength = 1200.f;
}

void UVHDashComponent::ActivateAbility()
{
	if (bIsOnCooldown) return;

	Super::ActivateAbility();

	OwnerCharacter = Cast<AVHPlayerCharacter>(GetOwner());
	if (!OwnerCharacter) return;

	
	MoveComp = OwnerCharacter->GetCharacterMovement();

	FVector DashDirection = MoveComp->GetLastInputVector().GetSafeNormal();
	
	if (DashDirection.IsNearlyZero())
	{
		DashDirection = OwnerCharacter->GetActorForwardVector();
	}
	DashDirection.Z = 0.f;
	DashDirection.Normalize();

	MoveComp->Velocity = FVector::ZeroVector;
	MoveComp->BrakingFrictionFactor = 0.f;
	
	OwnerCharacter->bisDashing = true;
	
	FTimerHandle timer;
	GetWorld()->GetTimerManager().SetTimer(timer, this, &UVHDashComponent::ResetFriction, RestoreCooldown, false);
	FVector DashVelocity = DashDirection * DashStrength;
	DashVelocity.Z = 0.f;
	OwnerCharacter->LaunchCharacter(DashVelocity, true, true);
	OwnerCharacter->StaminaComponent->ApplyStaminaCost(OwnerCharacter->DashCost);
	
	OnAbilityActivated();
}

void UVHDashComponent::ResetFriction()
{
	if (!MoveComp) return; 
	MoveComp->BrakingFrictionFactor = BrakingFrictionFactorValue;
}