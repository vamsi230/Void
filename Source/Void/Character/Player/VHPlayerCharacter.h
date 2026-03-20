// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Void/DamageSystem/Interfaces/VHDamageInterface.h"
#include "VHPlayerCharacter.generated.h"

class UVHHealComponent;
class UVHStaminaComponent;
class USceneComponent;
class UVHUltimateAbility;
class UVHRangedAbility;
class UVHDashComponent;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UVHHealthComponent;
class UVHCombatComponent;
UCLASS(Blueprintable)
class VOID_API AVHPlayerCharacter : public ACharacter, public IVHDamageInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVHPlayerCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Components")
	FTimerHandle SprintTimerHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Components")
	float SprintCallRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Components")
	UVHStaminaComponent* StaminaComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Stamina Cost")
	float DashCost;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Stamina Cost")
	float SprintCost;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Stamina Cost")
	float AttackCost;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Stamina Cost")
	float JumpCost;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Abilities") 
	UVHDashComponent* DashComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Abilities") 
	UVHRangedAbility* RangedAbility;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Abilities") 
	UVHUltimateAbility* UltimateAbility;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Combat") 
	UVHHealthComponent* HealthComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Combat") 
	UVHCombatComponent* CombatComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Combat") 
	UVHHealComponent* HealComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= "Dash")
	bool bisDashing;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Jump")
	class UAnimMontage* DeathAM;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Ranged Ability")
	UAnimMontage* CastAM;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Projectile")
	USceneComponent* ProjectileSpawnLocation;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Movement")
	bool bCanJump;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Camera")
	UCameraComponent* FollowCamera;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Camera")
	USpringArmComponent* SpringArm;
	
	UPROPERTY(EditAnywhere, Category= "Components")
	class AVHWaveManager* WaveManager;
	
	// Input Mapping Context
	UPROPERTY(EditAnywhere, Category= "EnhancedInput")
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, Category="EnhancedInput")
	UInputAction* MoveAction;
		
	UPROPERTY(EditAnywhere, Category= "EnhancedInput")
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, Category= "EnhancedInput")
	UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, Category= "EnhancedInput")
	UInputAction* SprintAction;
	
	UPROPERTY(EditAnywhere, Category= "EnhancedInput")
	UInputAction* RangedAbilityAction;
	
	UPROPERTY(EditAnywhere, Category= "EnhancedInput")
	UInputAction* DashAction;
	
	UPROPERTY(EditAnywhere, Category= "EnhancedInput")
	UInputAction* LightAttackAction;
	
	UPROPERTY(EditAnywhere, Category= "EnhancedInput")
	UInputAction* UltimateAbilityAction;
	
	UPROPERTY(EditAnywhere, Category= "EnhancedInput")
	UInputAction* HealAction;
	
	UPROPERTY(EditAnywhere, Category= "EnhancedInput")
	UInputAction* UltimateSwitchAction;
	
	UPROPERTY(EditAnywhere, Category= "EnhancedInput")
	UInputAction* SpawnWave;
	
	UPROPERTY(EditAnywhere, Category="Sprint")
	float SprintMaxSpeed;
	
	UPROPERTY(EditAnywhere, Category="Sprint")
	float SprintMinSpeed;
	
public:
	void SwitchUltimateType();
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Stats")
	bool bIsSprinting;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Stats")
	bool bIsShooting;
	
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Stats")
	bool bisDead;
	
	UPROPERTY(VisibleAnywhere, Category= "Stats")
	bool bCanDash;
	
	// Public interface - called from input or Blueprints
	void Move(const FInputActionValue& Value);
	void PlayerStartJump();
	void PlayerStopJump();
	void Look(const FInputActionValue& Value);
	void Attack();
	void UseRangedAbility();
	void UseUltimate();
	void StartWaves();
	void Heal();
	//Jump is handled by the inherited character class.
protected:
	// Overridable gameplay actions
	void StartSprinting();
	void DrainSprintStamina();
	void StopSprinting();
	void DashInput();

public:
	// Change these to use _Implementation
	UFUNCTION()
	virtual void TakeDamage_Implementation(float DamageAmount, FVector HitLocation) override;
	UFUNCTION()
	virtual void OnDeath_Implementation() override;

};
