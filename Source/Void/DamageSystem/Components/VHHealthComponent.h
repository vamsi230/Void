// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VHHealthComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType)
class VOID_API UVHHealthComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UVHHealthComponent();
public:
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category="Stats")
	float CurrentHealth;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Stats")
	float MaxHealth;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Stats")
	bool bIsDead;
	
	UFUNCTION(BlueprintCallable, Category= "Stats")
	void TakeDamage(float DamageAmount);

	
	UFUNCTION( BlueprintCallable, Category = "Stats")
	void Heal(float HealAmount);
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
	UPROPERTY(BlueprintAssignable, Category="Stats")
	FOnDeath OnDeath;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, CurrentHealth, float, MaxHealth);
	UPROPERTY(BlueprintAssignable, Category="Stats")
	FOnHealthChanged OnHealthChanged;
};
