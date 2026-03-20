// Fill out your copyright notice in the Description page of Project Settings.


#include "VHAbilityBase.h"

UVHAbilityBase::UVHAbilityBase()
{
	PrimaryComponentTick.bCanEverTick = false;
	bIsOnCooldown = false;
}

void UVHAbilityBase::ActivateAbility()
{
	if (bIsOnCooldown) return;
	
	bIsOnCooldown = true;
	
	OnCooldownUpdated.Broadcast(true, CooldownDuration);
	OnAbilityActivated();
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UVHAbilityBase::ResetCooldown, CooldownDuration, false);
	
}

void UVHAbilityBase::ResetCooldown()
{
	bIsOnCooldown = false;
	OnCooldownUpdated.Broadcast(false, 0.0f);
}