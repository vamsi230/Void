// Fill out your copyright notice in the Description page of Project Settings.


#include "VHUltimateAbility.h"


// Sets default values for this component's properties
UVHUltimateAbility::UVHUltimateAbility()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UVHUltimateAbility::BeginPlay()
{
	Super::BeginPlay();
	
}

void UVHUltimateAbility::ActivateAbility()
{
	if (bIsOnCooldown) return;
	
	Super::ActivateAbility();
	
	SelectType(UltimateType);
}



void UVHUltimateAbility::SwitchAbility()
{
	if (UltimateType == EChooseUltimate::Damage)
	{
		UltimateType = EChooseUltimate::Heal;
	}
	else
	{
		UltimateType = EChooseUltimate::Damage;
	}
}


void UVHUltimateAbility::SelectType(EChooseUltimate type)
{
	TSubclassOf<AVHUltimateActor> SpawnType;
	switch (type)
	{
	case EChooseUltimate::Heal:
		SpawnType = HealAOEActor;
		break;
	case EChooseUltimate::Damage:
		SpawnType = DamageAOEActor;
		break;
	default:
		return;
	}
	if (!SpawnType) return;
	
	FVector ForwardLocation = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 100.f;
	FHitResult HitResult;
	FVector Start = ForwardLocation + FVector(0, 0, 200);
	FVector End = ForwardLocation - FVector(0, 0, 1000);
	
	FCollisionQueryParams CollisionParameters;
	CollisionParameters.AddIgnoredActor(GetOwner());
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParameters))
	{
		GetWorld()->SpawnActor<AVHUltimateActor>(SpawnType, HitResult.Location, GetOwner()->GetActorRotation());
	}
}


