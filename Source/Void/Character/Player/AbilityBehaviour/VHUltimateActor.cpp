// Fill out your copyright notice in the Description page of Project Settings.


#include "VHUltimateActor.h"
#include "Void/DamageSystem/Interfaces/VHDamageInterface.h"
#include "Components/SphereComponent.h"
#include "Void/DamageSystem/Components/VHHealthComponent.h"

// Sets default values
AVHUltimateActor::AVHUltimateActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = SphereCollision;

	SphereCollision->SetGenerateOverlapEvents(true);
	SphereCollision->SetSphereRadius(SphereRadius);
	
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AVHUltimateActor::OnSphereBeginOverlap);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &AVHUltimateActor::OnSphereEndOverlap);
}

// Called when the game starts or when spawned
void AVHUltimateActor::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AVHUltimateActor::ApplyEffect, TickTime, true);
}

void AVHUltimateActor::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	if (!OtherActor || OtherActor == this)
	{
		return;
	}
	
	ActorsOverlapped.AddUnique(OtherActor);
}

void AVHUltimateActor::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ActorsOverlapped.Remove(OtherActor);
}

void AVHUltimateActor::ApplyEffect()
{
	if (UltimateType == EUltimateType::Damage)
	{
		for (AActor* Actor : ActorsOverlapped)
		{
			if (!IsValid(Actor)) continue;
			if (Actor->GetClass()->ImplementsInterface(UVHDamageInterface::StaticClass()))
			{
				IVHDamageInterface::Execute_TakeDamage(Actor, Amount, GetActorLocation());
			}
		}
	}
	
	if (UltimateType == EUltimateType::Heal)
	{
		for (AActor* Actor : ActorsOverlapped)
		{
			if (!IsValid(Actor)) continue;
			UVHHealthComponent* Health = Actor->FindComponentByClass<UVHHealthComponent>();
			if (Health)
			{
				Health->Heal(Amount);
			}
		}
	}
}

