// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Void/Framework/VHAbilityBase.h"
#include "VHHealComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VOID_API UVHHealComponent : public UVHAbilityBase
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UVHHealComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	virtual void ActivateAbility() override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Values")
	float HealAmount;
};
