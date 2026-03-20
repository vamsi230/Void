// Fill out your copyright notice in the Description page of Project Settings.


#include "VHRangedAbility.h"
#include "Void/Character/Player/VHPlayerCharacter.h"



UVHRangedAbility::UVHRangedAbility()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UVHRangedAbility::ActivateAbility()
{
	if (bIsOnCooldown || !ProjectileBP) return;

	Super::ActivateAbility(); // let base handle cooldown

	PlayerCharacter = Cast<AVHPlayerCharacter>(GetOwner());
	if (!PlayerCharacter || !PlayerCharacter->ProjectileSpawnLocation) return;
	
	PlayerCharacter->PlayAnimMontage(PlayerCharacter->CastAM);
	
	if (!PlayerCharacter || !PlayerCharacter->GetMesh()) return;
	
	FVector SpawnLocation = PlayerCharacter->GetMesh()->GetSocketLocation("CastSpawnLocation");
	FRotator SpawnRotation = PlayerCharacter->GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = PlayerCharacter;
	SpawnParams.Instigator = PlayerCharacter;

	GetWorld()->SpawnActor<AActor>(ProjectileBP, SpawnLocation, SpawnRotation, SpawnParams);
}

