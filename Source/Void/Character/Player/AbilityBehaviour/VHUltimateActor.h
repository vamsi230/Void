// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VHUltimateActor.generated.h"


UENUM(BlueprintType)
enum class EUltimateType : uint8
{
	Damage,
	Heal,
	None
};


UCLASS()
class VOID_API AVHUltimateActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AVHUltimateActor();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="General")
	class USphereComponent* SphereCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="General")
	TArray<AActor*> ActorsOverlapped;
	
	FTimerHandle TimerHandle;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="General")
	EUltimateType UltimateType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="General")
	float SphereRadius = 50.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="General")
	float TickTime = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="General")
	float lifetime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="General")
	float Amount;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UFUNCTION()
	void OnSphereBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);
	
	UFUNCTION()
	void ApplyEffect();
};
