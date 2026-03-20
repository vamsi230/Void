// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VHCombatComponent.generated.h"


USTRUCT(Blueprintable)
struct FSocketPair
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Detection")
	FName SocketStartName;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Detection")
	FName SocketEndName;
};
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VOID_API UVHCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UVHCombatComponent();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Type")
	bool isEnemy;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Type")
	bool isPlayer;
	
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Animation")
	TArray<UAnimMontage*> AttackMontages;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Combat")
	float AttackCooldown;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Animation")
	int AttackIndexCurrent;
	UFUNCTION(Blueprintable)
	void ResetHitStopTimer();
	UFUNCTION(Blueprintable)
	void PlayAttackMontage();
	void PlayEnemyAttackMontage();
	UFUNCTION(Blueprintable)
	void PlayHitReact(FVector HitLocation);
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Combat")
	class UVHHealthComponent* HealthComponent;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Combat")
	class AVHPlayerCharacter* CharacterMeshRef;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Combat")
	bool bCanAttack = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	TArray<FSocketPair> TraceSockets;
	
	FQuat CapsuleRotation;
	
	UFUNCTION()
	void StartMeleeDetection();
	UFUNCTION()
	void EndMeleeDetection();
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Detection")
	bool bMeleeDetectionActive;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	float CapsuleRadius = 30.f;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	float CapsuleHalfHeight = 120.f;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Detection")
	TSet<AActor*> HitActors;
	
	

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Detection")
	float MeleeRange = 150.f;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Detection")
	FVector PreviousSocketLocation;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Detection")
	bool bHasPreviousSocket = false;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Detection")
	bool bTraceVisibility;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="FX")
	class USoundBase* ImpactSound;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="FX")
	class USoundBase* ImpactEnemyHitSound;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="FX")
	class UParticleSystem* ImpactVFX;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="FX")
	float HitStopDilation;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Animation")
	UAnimMontage* JumpMontage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="HitReactMontages")
	UAnimMontage* FrontHitReactMontage;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="HitReactMontages")
	UAnimMontage* BackHitReactMontage;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="HitReactMontages")
	UAnimMontage* RightHitReactMontage;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="HitReactMontages")
	UAnimMontage* LeftHitReactMontage;
	
	FVector PreviousStart;
	FVector PreviousEnd;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void ApplyHitStop(float Duration);
	void ResetAttack();
	void ResetCombo();
	
	UPROPERTY(EditAnywhere)
	float AttackDamage = 20.0f;
    
	UPROPERTY(EditAnywhere)
	float AttackRange = 200.0f;

};
