// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VHStaminaComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VOID_API UVHStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UVHStaminaComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	

public:

	UFUNCTION()
	bool DrainStamina(float DrainRate);
	UFUNCTION()
	void RegenStamina();
	UFUNCTION()
	bool StaminaCost(float cost);
	UFUNCTION()
	void RestoreRegen();
	UFUNCTION()
	void ApplyStaminaCost(float cost);
	


	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChanged, float, CurrentStamina, float, MaxStamina);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Stamina")
	float CurrentStamina;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Stamina")
	float MaxStamina;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Stamina")
	float RegenRate;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Stamina")
	float TickRateCooldown;
	
	UPROPERTY(BlueprintAssignable, Category="Stamina")
	FOnStaminaChanged OnStaminaChanged;
	FTimerHandle StaminaTimerHandle;
	
	FTimerHandle RegenTimerHandle;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Stamina")
	float RegenDelay;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Stamina")
	bool bIsStaminaConsumed;
};
