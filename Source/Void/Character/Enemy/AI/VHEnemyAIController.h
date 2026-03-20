// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "VHEnemyAIController.generated.h"




UCLASS()
class VOID_API AVHEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AVHEnemyAIController();
	
	UPROPERTY(EditAnywhere, Category="AI")
	UBehaviorTree* BehaviorTree;
    
	UPROPERTY(EditAnywhere, Category="AI")
	UBlackboardData* BlackboardAsset;
	
	
	UPROPERTY(EditAnywhere, Category="AI")
	FPerceptionUpdatedDelegate PereceptionUpdated;
	
	UPROPERTY(EditDefaultsOnly, Category="AI")
	FName TargetActorKey = "TargetPlayer";
};
