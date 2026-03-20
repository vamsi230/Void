// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VHWaveManager.generated.h"




USTRUCT(BlueprintType)
struct FWave
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AVHBaseEnemyCharacter> MeleeEnemy;
	
	UPROPERTY(EditAnywhere)
	int32 MeleeEnemyCount;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AVHBaseEnemyCharacter> RangedEnemy;
	
	UPROPERTY(EditAnywhere)
	int32 RangedEnemyCount;
};

UCLASS()
class VOID_API AVHWaveManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AVHWaveManager();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="General")
	TArray<AActor*> EnemySpawners;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="General")
	TArray<FWave> Waves;
	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="General")
	int32 CurrentWave = 0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="General")		
	int32 AliveEnemies = 0;
	
	UFUNCTION(Blueprintable)
	void SpawnWave();
	UFUNCTION(Blueprintable)
	bool HandleSpawn(TSubclassOf<AVHBaseEnemyCharacter> EnemyType, FVector SpawnLocation);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpawnWaveDelegate, float, CurrentWaveIndex);
	UPROPERTY(BlueprintAssignable)
	FOnSpawnWaveDelegate OnSpawnWave;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWavesEnded);
	UPROPERTY(BlueprintAssignable)
	FOnWavesEnded OnWavesEnded;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="General")
	int32 MaxAttempts;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:
	void NotifyEnemyKilled();
	void NextWave();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="General")
	bool bDidGameEnd;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="General")
	float WaveCooldown = 2.f;

	FTimerHandle WaveTimerHandle;
};
