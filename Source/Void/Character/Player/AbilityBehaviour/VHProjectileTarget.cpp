// Fill out your copyright notice in the Description page of Project Settings.


#include "VHProjectileTarget.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Void/Character/Enemy/VHBaseEnemyCharacter.h"


// Sets default values for this component's properties
UVHProjectileTarget::UVHProjectileTarget()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UVHProjectileTarget::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMovementComponent = GetOwner()->FindComponentByClass<UProjectileMovementComponent>();
	if (!ProjectileMovementComponent) return;
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVHBaseEnemyCharacter::StaticClass(), EnemyCharacters);
	
	AActor* ClosestEnemy = nullptr;
	float MinDistance = FLT_MAX;
	
	for (AActor* Enemy : EnemyCharacters)
	{
		AVHBaseEnemyCharacter* EnemyCharacter = Cast<AVHBaseEnemyCharacter>(Enemy);
		if (!EnemyCharacter) continue;

		float Distance = FVector::Dist(GetOwner()->GetActorLocation(), EnemyCharacter->GetActorLocation());

		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			ClosestEnemy = EnemyCharacter;
		}
	}
	
	TargetActor = ClosestEnemy;
	if (!TargetActor) return;
	ProjectileMovementComponent->HomingTargetComponent = TargetActor->GetRootComponent();
}
