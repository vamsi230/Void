// Fill out your copyright notice in the Description page of Project Settings.


#include "VHCombatComponent.h"
#include "GameFramework/Character.h"
#include "Void/Character/Player/VHPlayerCharacter.h"
#include "DrawDebugHelpers.h"
#include "VHStaminaComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Void/Character/Enemy/VHBaseEnemyCharacter.h"

// In constructor initialize
UVHCombatComponent::UVHCombatComponent() : AttackIndexCurrent(0)
{
	PrimaryComponentTick.bCanEverTick = true;
	bMeleeDetectionActive = false;
}


void UVHCombatComponent::StartMeleeDetection()
{
	bMeleeDetectionActive = true;
	HitActors.Empty();
}

void UVHCombatComponent::EndMeleeDetection()
{
	bMeleeDetectionActive = false;
}

// Called when the game starts
void UVHCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle ResetAttackTimerHandle;
	
	GetWorld()->GetTimerManager().SetTimer(ResetAttackTimerHandle, this, &UVHCombatComponent::ResetAttack, AttackCooldown, true);
}


// Called every frame
void UVHCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!bMeleeDetectionActive) return;
	
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;
	
	FCollisionShape Shape = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Character);
	Params.bTraceComplex = false;
	for (const FSocketPair& SocketPair : TraceSockets)
	{
		TArray<FHitResult> Hits;
		
		FVector Start = Character->GetMesh()->GetSocketLocation(SocketPair.SocketStartName);
		FVector End = Character->GetMesh()->GetSocketLocation(SocketPair.SocketEndName);

		FVector Direction = (End - Start).GetSafeNormal();
		CapsuleRotation = FRotationMatrix::MakeFromZ(Direction).ToQuat();
		
		GetWorld()->SweepMultiByChannel(
			Hits,
			Start,
			End,
			CapsuleRotation,
			ECC_Pawn,
			Shape,
			Params
		);
		
		FVector Center = (Start + End) * 0.5f;
		float HalfHeight = (End - Start).Size() * 0.5f + CapsuleHalfHeight;

		if (bTraceVisibility)
		{
			DrawDebugCapsule(GetWorld(), Center, HalfHeight, CapsuleRadius, CapsuleRotation, FColor::Red, false, 1.f, 0, 1.f	);
		}
		
		for (FHitResult& Hit : Hits)
		{
			AActor* HitActor = Hit.GetActor();	
		
			if (!HitActor || HitActors.Contains(HitActor)) continue;
			
			HitActors.Add(HitActor);
			if (HitActor->GetClass()->ImplementsInterface(UVHDamageInterface::StaticClass()))
			{
				if (!IsValid(HitActor)) continue;

				if (isEnemy)
				{
					if (AVHPlayerCharacter* Player = Cast<AVHPlayerCharacter>(HitActor))
					{
						IVHDamageInterface::Execute_TakeDamage(Player, AttackDamage, Hit.ImpactPoint);
					}
				}
				else if (isPlayer)
				{
					if (AVHBaseEnemyCharacter* Enemy = Cast<AVHBaseEnemyCharacter>(HitActor))
					{
						IVHDamageInterface::Execute_TakeDamage(Enemy, AttackDamage, Hit.ImpactPoint);
					}
				}
				else continue;
			}
			if (ImpactVFX)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactVFX, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			}
		
			ACharacter* Enemy  = Cast<ACharacter>(HitActor);
			if (Enemy)
			{
				if (ImpactEnemyHitSound)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactEnemyHitSound, Hit.ImpactPoint);
				}
				ApplyHitStop(0.04f);
			}
			else
			{
				if (ImpactSound)
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.ImpactPoint);
				}
			}
		}
	}
	
}

void UVHCombatComponent::ApplyHitStop(float Duration)
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), HitStopDilation);
	
	FTimerHandle TimerHandle;
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UVHCombatComponent::ResetHitStopTimer, Duration, false);
}

void UVHCombatComponent::ResetHitStopTimer()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
}

void UVHCombatComponent::PlayAttackMontage()
{
	AVHPlayerCharacter* Character = Cast<AVHPlayerCharacter>(GetOwner());
	if (!Character) return;
	if (!bCanAttack) return;
	
	if (bCanAttack && Character->GetCharacterMovement()->IsFalling())
	{
		Character->PlayAnimMontage(JumpMontage);
		ResetCombo();
		return;
	}
	bCanAttack = false;
	if (AttackMontages.IsValidIndex(AttackIndexCurrent))
	{
		Character->PlayAnimMontage(AttackMontages[AttackIndexCurrent]);
		AttackIndexCurrent = (AttackIndexCurrent + 1) % AttackMontages.Num();
	}
}

void UVHCombatComponent::PlayEnemyAttackMontage()
{
	AVHBaseEnemyCharacter* EnemyCharacter = Cast<AVHBaseEnemyCharacter>(GetOwner());
	if (!EnemyCharacter) return;
	
	if (EnemyCharacter->GetCharacterMovement()->IsFalling())
	{
		ResetCombo();
	}
	
	if (AttackMontages.IsValidIndex(AttackIndexCurrent))
	{
		EnemyCharacter->PlayAnimMontage(AttackMontages[FMath::RandRange(0, AttackMontages.Num()-1)]);
	}
}

void UVHCombatComponent::PlayHitReact(FVector HitLocation)
{
	FVector Forward = GetOwner()->GetActorForwardVector();
	FVector ToHit = (HitLocation - GetOwner()->GetActorLocation()).GetSafeNormal();
	
	float Dot = FVector::DotProduct(Forward, ToHit);
	float Cross = FVector::CrossProduct(Forward, ToHit).Z;
	
	UAnimMontage* MontageToPlay;
	
	if (Dot > 0.5f)
	{
		MontageToPlay = FrontHitReactMontage;
	}
	else if (Dot < -0.5f)
	{
		MontageToPlay = BackHitReactMontage;
	}
	else if (Cross < 0.f)
	{
		MontageToPlay = RightHitReactMontage;
	}
	else
	{
		MontageToPlay = LeftHitReactMontage;
	}
	
	if (MontageToPlay)
	{
		ACharacter* Character = Cast<ACharacter>(GetOwner());
		if (!Character) return;
		Character->PlayAnimMontage(MontageToPlay);
	}
}

void UVHCombatComponent::ResetAttack()
{
	bCanAttack = true;
}

void UVHCombatComponent::ResetCombo()
{
	AttackIndexCurrent = 0;
}

