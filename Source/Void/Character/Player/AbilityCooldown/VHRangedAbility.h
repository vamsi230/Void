// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Void/Framework/VHAbilityBase.h"
#include "VHRangedAbility.generated.h"


class AVHPlayerCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VOID_API UVHRangedAbility : public UVHAbilityBase
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UVHRangedAbility();
	
public:

	virtual void ActivateAbility() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Abilities")
	AVHPlayerCharacter* PlayerCharacter;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Abilities")
	TSubclassOf<AActor> ProjectileBP;
};
