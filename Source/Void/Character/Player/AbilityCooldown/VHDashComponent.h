// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Void/Framework/VHAbilityBase.h"
#include "VHDashComponent.generated.h"

class AVHPlayerCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VOID_API UVHDashComponent : public UVHAbilityBase
{
	GENERATED_BODY()
	
public:
	UVHDashComponent();
	
	virtual void ActivateAbility() override;
	void ResetFriction();

	UPROPERTY()
	AVHPlayerCharacter* OwnerCharacter;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Abilities")
	float DashStrength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Abilities")
	float RestoreCooldown;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Abilities")
	class UCharacterMovementComponent* MoveComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Abilities")
	float BrakingFrictionFactorValue;
};
