// Fill out your copyright notice in the Description page of Project Settings.


#include "VHWaveManager.h"

#include "Void/Character/Enemy/VHBaseEnemyCharacter.h"


// Sets default values
AVHWaveManager::AVHWaveManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}



void AVHWaveManager::SpawnWave()
{
	if (EnemySpawners.Num() == 0) return;
	if (!Waves.IsValidIndex(CurrentWave)) return;
	
	FWave& Wave = Waves[CurrentWave];
	
	OnSpawnWave.Broadcast(CurrentWave);
	
	int32 ActuallySpawned = 0;

	// Spawn Ranged
	for (int32 i = 0; i < Wave.RangedEnemyCount; i++)
	{
		if (!Wave.RangedEnemy) continue;

		int32 Index = FMath::RandRange(0, EnemySpawners.Num() - 1);
		AActor* Spawner = EnemySpawners[Index];

		if (!Spawner) continue;

		if (HandleSpawn(Wave.RangedEnemy, Spawner->GetActorLocation()))
		{
			ActuallySpawned++;
		}
	}

	// Spawn Melee
	for (int32 i = 0; i < Wave.MeleeEnemyCount; i++)
	{
		if (!Wave.MeleeEnemy) continue;

		int32 Index = FMath::RandRange(0, EnemySpawners.Num() - 1);
		AActor* Spawner = EnemySpawners[Index];

		if (!Spawner) continue;

		if (HandleSpawn(Wave.MeleeEnemy, Spawner->GetActorLocation()))
		{
			ActuallySpawned++;
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Spawning Wave %d. Total enemies spawned: %d"), CurrentWave + 1, ActuallySpawned);
	AliveEnemies = ActuallySpawned;

	// If somehow no enemies spawned, go to next wave automatically
	if (AliveEnemies <= 0)
	{
		NotifyEnemyKilled();
	}
}



bool AVHWaveManager::HandleSpawn(TSubclassOf<AVHBaseEnemyCharacter> EnemyType, FVector SpawnLocation)
{
	if (!EnemyType) return false;

	UWorld* World = GetWorld();
	if (!World) return false;

	MaxAttempts = 5;

	for (int32 Attempt = 0; Attempt < MaxAttempts; Attempt++)
	{
		AVHBaseEnemyCharacter* Enemy = World->SpawnActor<AVHBaseEnemyCharacter>(
			EnemyType,
			SpawnLocation,
			FRotator::ZeroRotator
		);

		if (Enemy)
		{
			Enemy->WaveManager = this;
			return true; // success
		}

		UE_LOG(LogTemp, Warning, TEXT("Spawn attempt %d failed"), Attempt + 1);
	}

	return false; // all attempts failed
}

// Called when the game starts or when spawned
void AVHWaveManager::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("WaveManager initialized with %d waves"), Waves.Num());
	for (int32 i = 0; i < Waves.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Wave %d: Melee=%d, Ranged=%d"), 
			i, Waves[i].MeleeEnemyCount, Waves[i].RangedEnemyCount);
	}
	
	if (CurrentWave > Waves.Num())
	{
		bDidGameEnd = false;
	}
}


void AVHWaveManager::NotifyEnemyKilled()
{
	AliveEnemies--;
	if (AliveEnemies < 0) AliveEnemies = 0;

	UE_LOG(LogTemp, Warning, TEXT("Enemy killed. Remaining: %d"), AliveEnemies);

	if (AliveEnemies <= 0)
	{
		// Only start timer and increment wave if a transition isn't already in progress
		if (!GetWorldTimerManager().IsTimerActive(WaveTimerHandle))
		{
			CurrentWave++;
			GetWorldTimerManager().SetTimer(WaveTimerHandle, this, &AVHWaveManager::NextWave, WaveCooldown, false);
		}
	}
}

void AVHWaveManager::NextWave()
{
	if (CurrentWave < Waves.Num())
	{
		bDidGameEnd = false;
		SpawnWave();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("All waves completed! Total waves: %d"), Waves.Num());
		OnWavesEnded.Broadcast();
		bDidGameEnd = true;
	}
}

