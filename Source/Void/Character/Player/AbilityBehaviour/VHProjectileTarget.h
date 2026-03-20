// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VHProjectileTarget.generated.h"


class AVHBaseEnemyCharacter;
class UProjectileMovementComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VOID_API UVHProjectileTarget : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UVHProjectileTarget();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="target")
	AActor* TargetActor;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Settings")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	TArray<AActor*> EnemyCharacters;
};
