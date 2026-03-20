// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Void/DamageSystem/Interfaces/VHDamageInterface.h"
#include "VHBaseEnemyCharacter.generated.h"


class UVHCombatComponent;
class AVHWaveManager;
class UVHHealthComponent;

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	Melee,
	Ranged
};

UCLASS(Blueprintable)
class VOID_API AVHBaseEnemyCharacter : public ACharacter, public IVHDamageInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVHBaseEnemyCharacter();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "State")
	bool bIsDead = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy")
	EEnemyType EnemyType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Widgets")
	class UWidgetComponent* HealthBarWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Widgets")
	FName WidgetSocketName;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ProjectileClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float AttackCooldown = 1.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	float LastAttackTime = -100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UVHCombatComponent* CombatComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	AActor* TargetActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	UVHHealthComponent* HealthComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WaveData")
	AVHWaveManager* WaveManager;
    
	UPROPERTY(EditAnywhere, Category="AI")
	float AttackRange = 900.0f;
	
	UPROPERTY(EditAnywhere, Category="Shoot")
	class USceneComponent* SpawnLocation;
	
	UPROPERTY(EditAnywhere, Category="Common")
	float MinDistance = 400.f;
	FTimerHandle ThinkTimer;


	UFUNCTION(Blueprintable)
	void ShootProjectile();
	UFUNCTION(Blueprintable)
	void MoveAwayFromTarget();
	UFUNCTION(Blueprintable)
	virtual void TakeDamage_Implementation(float DamageAmount, FVector HitLocation) override;
	
	UFUNCTION(Blueprintable)
	virtual void OnDeath_Implementation() override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void think();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void MoveToTarget();

	void Attack();
};
