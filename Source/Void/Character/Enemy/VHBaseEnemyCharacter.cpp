// Fill out your copyright notice in the Description page of Project Settings.


#include "VHBaseEnemyCharacter.h"

#include "AI/VHEnemyAIController.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Void/Character/Player/AbilityBehaviour/VHProjectileTarget.h"
#include "Void/DamageSystem/Components/VHHealthComponent.h"
#include "Void/DamageSystem/Components/VHCombatComponent.h"
#include "Void/Framework/VHWaveManager.h"


// Sets default values
AVHBaseEnemyCharacter::AVHBaseEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	HealthComponent = CreateDefaultSubobject<UVHHealthComponent>(TEXT("HealthComponent"));
	
	SpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnLocation"));
	
	SpawnLocation->SetupAttachment(GetMesh(), TEXT("gun_slideSocket"));
	
	CombatComponent = CreateDefaultSubobject<UVHCombatComponent>(TEXT("CombatComponent"));
	
	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(GetMesh(), TEXT("HealthBar"));
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AVHBaseEnemyCharacter::TakeDamage_Implementation(float DamageAmount, FVector HitLocation)
{
	IVHDamageInterface::TakeDamage_Implementation(DamageAmount, HitLocation);
	if (HealthComponent)
	{
		HealthComponent->TakeDamage(DamageAmount);
	}
	if (CombatComponent)
	{
		CombatComponent->PlayHitReact(HitLocation);
	}
}

void AVHBaseEnemyCharacter::OnDeath_Implementation()
{
	IVHDamageInterface::OnDeath_Implementation();

	bIsDead = true;

	// STOP AI COMPLETELY FIRST
	GetWorldTimerManager().ClearTimer(ThinkTimer);

	if (AController* C = GetController())
	{
		C->StopMovement();
		C->UnPossess();
	}
	

	if (WaveManager)
	{
		WaveManager->NotifyEnemyKilled();
	}
}

// Called when the game starts or when spawned
void AVHBaseEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &AVHBaseEnemyCharacter::OnDeath);
	}
	TargetActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
	if (CombatComponent)
	{
		CombatComponent->isEnemy = true;
		CombatComponent->isPlayer = false;
	}

	GetWorldTimerManager().SetTimer(
		ThinkTimer,
		this,
		&AVHBaseEnemyCharacter::think,
		0.25f,
		true
	);
}

void AVHBaseEnemyCharacter::think()
{
	if (!TargetActor) return;
	if (bIsDead) return;
	float Distance = FVector::Dist(GetActorLocation(), TargetActor->GetActorLocation());
	float Time = GetWorld()->GetTimeSeconds();

	if (EnemyType == EEnemyType::Melee)
	{
		if (Distance <= AttackRange)
		{
			if (Time - LastAttackTime >= AttackCooldown)
			{
				Attack();
				LastAttackTime = Time;
			}
		}
		else
		{
			MoveToTarget();
		}
	}

	if (EnemyType == EEnemyType::Ranged)
	{
		if (Distance > AttackRange)
		{
			MoveToTarget();
		}
		else if (Distance < MinDistance)
		{
			MoveAwayFromTarget();
		}
		else
		{
			if (Time - LastAttackTime >= AttackCooldown)
			{
				Attack();
				LastAttackTime = Time;
			}
		}
	}
}
// Called every frame
void AVHBaseEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
}
void AVHBaseEnemyCharacter::MoveToTarget()
{
	if (!Controller) return;

	AAIController* AI = Cast<AVHEnemyAIController>(Controller);
	if (!AI) return;

	AI->MoveToActor(TargetActor, 150.f);
}

void AVHBaseEnemyCharacter::Attack()
{
	FVector Dir = TargetActor->GetActorLocation() - GetActorLocation();
	Dir.Z = 0;
	SetActorRotation(Dir.Rotation());
	AAIController* AI = Cast<AAIController>(Controller);
	if (AI)
	{
		AI->StopMovement();
	}
	if (EnemyType == EEnemyType::Melee)
	{
		if (CombatComponent)
		{
			CombatComponent->PlayEnemyAttackMontage();
		}
	}
	else if (EnemyType == EEnemyType::Ranged)
	{
		ShootProjectile();
	}
	
}

void AVHBaseEnemyCharacter::ShootProjectile()
{
	if (!ProjectileClass || !TargetActor) return;
	
	FVector SpawnDirection = (TargetActor->GetActorLocation() - GetActorLocation() + GetActorForwardVector() * 100.f).GetSafeNormal();
	FRotator Rotation = SpawnDirection.Rotation();

	AActor* Projectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation->GetComponentLocation(), Rotation);

	if (Projectile)
	{
		UVHProjectileTarget* TargetComp = Projectile->FindComponentByClass<UVHProjectileTarget>();
		if (TargetComp)
		{
			TargetComp->TargetActor = TargetActor;
		}
	}
}

void AVHBaseEnemyCharacter::MoveAwayFromTarget()
{
	if (!Controller || !TargetActor) return;

	FVector Direction = (GetActorLocation() - TargetActor->GetActorLocation()).GetSafeNormal();
	FVector NewLocation = GetActorLocation() + Direction * 300.f;

	NewLocation += FVector(FMath::RandRange(-200,200), FMath::RandRange(-200,200), 0);
	AAIController* AI = Cast<AAIController>(Controller);
	if (AI)
	{
		AI->MoveToLocation(NewLocation);
	}
}

