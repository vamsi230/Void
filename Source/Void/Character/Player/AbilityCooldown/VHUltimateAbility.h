// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Void/Framework/VHAbilityBase.h"
#include "Void/Character/Player/AbilityBehaviour/VHUltimateActor.h"
#include "VHUltimateAbility.generated.h"


UENUM(BlueprintType)
enum class EChooseUltimate : uint8
{
	Damage,
	Heal
};	

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VOID_API UVHUltimateAbility : public UVHAbilityBase
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UVHUltimateAbility();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Type")
	TSubclassOf<AVHUltimateActor> HealAOEActor;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Type")
	TSubclassOf<AVHUltimateActor> DamageAOEActor;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Type")
	EChooseUltimate UltimateType;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:
	virtual void ActivateAbility() override;
	void SwitchAbility();

	UFUNCTION()
	void SelectType(EChooseUltimate type);
	
};
