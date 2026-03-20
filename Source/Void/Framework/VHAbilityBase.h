// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VHAbilityBase.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable, Abstract)
class VOID_API UVHAbilityBase : public UActorComponent
{
	GENERATED_BODY()
    
public:
	UVHAbilityBase();
	
	UFUNCTION(BlueprintCallable, Category="Ability")
	virtual void ActivateAbility();
    
	UFUNCTION(BlueprintImplementableEvent, Category="Ability")
	void OnAbilityActivated();  // BP implementation for effects
    
	
	FTimerHandle TimerHandle;
	UFUNCTION(BlueprintCallable, Category="Ability")
	void ResetCooldown();
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCooldownUpdated, bool, bIsOnCooldown, float, RemainingTime);
	UPROPERTY(BlueprintAssignable, Category="Ability")
	FOnCooldownUpdated OnCooldownUpdated;
    
protected:
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	float CooldownDuration = 1.0f;
    
	UPROPERTY(BlueprintReadOnly, Category="Ability")
	bool bIsOnCooldown = false;
};
