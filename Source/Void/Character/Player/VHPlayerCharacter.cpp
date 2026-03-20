// Fill out your copyright notice in the Description page of Project Settings.


#include "VHPlayerCharacter.h"

#include "Camera/CameraComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "AbilityCooldown/VHDashComponent.h"
#include "AbilityCooldown/VHHealComponent.h"
#include "AbilityCooldown/VHRangedAbility.h"
#include "AbilityCooldown/VHUltimateAbility.h"
#include "Kismet/GameplayStatics.h"

#include "Void/DamageSystem/Components/VHHealthComponent.h"
#include "Void/DamageSystem/Components/VHCombatComponent.h"
#include "Void/DamageSystem/Components/VHStaminaComponent.h"
#include "Void/Framework/VHWaveManager.h"


// Sets default values
AVHPlayerCharacter::AVHPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bIsSprinting = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	//Abilities
	DashComponent = CreateDefaultSubobject<UVHDashComponent>(TEXT("DashComponent"));
	RangedAbility = CreateDefaultSubobject<UVHRangedAbility>(TEXT("RangedAbility"));
	UltimateAbility = CreateDefaultSubobject<UVHUltimateAbility>(TEXT("UltimateAbility"));
	
	//Combat
	HealthComponent = CreateDefaultSubobject<UVHHealthComponent>(TEXT("HealthComp"));
	CombatComponent = CreateDefaultSubobject<UVHCombatComponent>(TEXT("CombatComp"));
	StaminaComponent = CreateDefaultSubobject<UVHStaminaComponent>(TEXT("StaminaComponent"));
	HealComponent = CreateDefaultSubobject<UVHHealComponent>(TEXT("HealComponent"));

	bisDashing = false;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bUsePawnControlRotation = true;
	
	
	ProjectileSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnLocation"));
	ProjectileSpawnLocation->SetupAttachment(GetMesh());
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	// Inside AMyCharacter::AMyCharacter()
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 300.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 150.0f;

	GetCharacterMovement()->MaxAcceleration = 2048.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2048.0f;
	GetCharacterMovement()->BrakingFriction = 8.0f;
	GetCharacterMovement()->BrakingFrictionFactor = 2.0f;

	GetCharacterMovement()->AirControl = 0.f;
	GetCharacterMovement()->AirControlBoostMultiplier = 2.0f;
	GetCharacterMovement()->AirControlBoostVelocityThreshold = 600.0f;

	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->GravityScale = 1.0f;
	GetCharacterMovement()->FallingLateralFriction = 0.5f;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	
	bReplicates = true;
	bisDashing = false;
	bCanDash = true;
}

// Called when the game starts or when spawned
void AVHPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	
	WaveManager = Cast<AVHWaveManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AVHWaveManager::StaticClass()));
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &AVHPlayerCharacter::OnDeath_Implementation);
	}
	
	if (CombatComponent)
	{
		CombatComponent->isPlayer = true;
		CombatComponent->isEnemy = false;
	}
}


// Called to bind functionality to input
void AVHPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Move
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AVHPlayerCharacter::Move);
	
		//Look
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AVHPlayerCharacter::Look);
	
		//Jump
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &AVHPlayerCharacter::PlayerStartJump);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Canceled, this, &AVHPlayerCharacter::PlayerStopJump);
	
		//Sprint
		EnhancedInput->BindAction(SprintAction, ETriggerEvent::Started, this, &AVHPlayerCharacter::StartSprinting);
		EnhancedInput->BindAction(SprintAction, ETriggerEvent::Completed, this, &AVHPlayerCharacter::StopSprinting);
	
		//Attack
		EnhancedInput->BindAction(LightAttackAction, ETriggerEvent::Started, this, &AVHPlayerCharacter::Attack);
	
		//Dash
		EnhancedInput->BindAction(DashAction, ETriggerEvent::Started, this, &AVHPlayerCharacter::DashInput);
	
		//Ability
		EnhancedInput->BindAction(RangedAbilityAction, ETriggerEvent::Started, this, &AVHPlayerCharacter::UseRangedAbility);
	
		//Ultimate
		EnhancedInput->BindAction(UltimateAbilityAction, ETriggerEvent::Started, this, &AVHPlayerCharacter::UseUltimate);
		EnhancedInput->BindAction(UltimateSwitchAction, ETriggerEvent::Started, this, &AVHPlayerCharacter::SwitchUltimateType);
		
		//Wave
		EnhancedInput->BindAction(SpawnWave, ETriggerEvent::Started, this, &AVHPlayerCharacter::StartWaves);
		
		//Heal
		EnhancedInput->BindAction(HealAction, ETriggerEvent::Started, this, &AVHPlayerCharacter::Heal);
	};
}

void AVHPlayerCharacter::Move(const FInputActionValue& Value)
{
	if (!Controller) return;

	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (MovementVector.IsNearlyZero()) return;

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection   = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection,  MovementVector.X);
}

void AVHPlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxis = Value.Get<FVector2D>();

	if (LookAxis.IsNearlyZero())
		return;

	AddControllerYawInput(LookAxis.X);
	AddControllerPitchInput(LookAxis.Y);
}

void AVHPlayerCharacter::PlayerStartJump()
{
	if (bCanJump && StaminaComponent->StaminaCost(JumpCost))
	{
		Jump();
	}
}

void AVHPlayerCharacter::PlayerStopJump()
{
	StopJumping();
}

void AVHPlayerCharacter::StartSprinting()
{
	GetWorld()->GetTimerManager().SetTimer(SprintTimerHandle, this, &AVHPlayerCharacter::DrainSprintStamina, SprintCallRate, true);
	GetCharacterMovement()->MaxWalkSpeed = SprintMaxSpeed;
}

void AVHPlayerCharacter::DrainSprintStamina()
{
	if (StaminaComponent->DrainStamina(SprintCost))
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintMaxSpeed;
		StaminaComponent->ApplyStaminaCost(SprintCost);
		bIsSprinting = true;
	}
	else
	{
		StopSprinting();
	}
}

void AVHPlayerCharacter::StopSprinting()
{
	GetWorld()->GetTimerManager().ClearTimer(SprintTimerHandle);
	StaminaComponent->bIsStaminaConsumed = false;
	GetCharacterMovement()->MaxWalkSpeed = SprintMinSpeed;
	bIsSprinting = false;
}

void AVHPlayerCharacter::DashInput()
{
	if (DashComponent && bCanDash && StaminaComponent->StaminaCost(DashCost))
	{
		DashComponent->ActivateAbility();
	}
}

void AVHPlayerCharacter::UseRangedAbility()
{
	if (RangedAbility)
	{
		RangedAbility->ActivateAbility();
	}
}

void AVHPlayerCharacter::UseUltimate()
{
	if (UltimateAbility)
	{
		UltimateAbility->ActivateAbility();
	}
}

void AVHPlayerCharacter::StartWaves()
{
	if (WaveManager)
	{
		WaveManager->SpawnWave();
	}
}

void AVHPlayerCharacter::Heal()
{
	if (HealComponent)
	{
		HealComponent->ActivateAbility();
	}
}

void AVHPlayerCharacter::SwitchUltimateType()
{
	if (UltimateAbility)
	{
		UltimateAbility->SwitchAbility();
	}
}

void AVHPlayerCharacter::Attack()
{
	if (CombatComponent && StaminaComponent->StaminaCost(AttackCost))
	{
		CombatComponent->PlayAttackMontage();
	}
}

void AVHPlayerCharacter::TakeDamage_Implementation(float DamageAmount, FVector HitLocation)
{
	IVHDamageInterface::TakeDamage_Implementation(DamageAmount, HitLocation);
	if (HealthComponent)
	{
		CombatComponent->PlayHitReact(HitLocation);
		HealthComponent->TakeDamage(DamageAmount);
	}
}

void AVHPlayerCharacter::OnDeath_Implementation()
{
	if (HealthComponent && HealthComponent->CurrentHealth <= 0)
	{
		OnDeath.Broadcast();
		GetCharacterMovement()->DisableMovement();
		GetController()->StopMovement();
	}
}


