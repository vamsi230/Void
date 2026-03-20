// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_MeleeAttack.h"

#include "Void/Character/Player/VHPlayerCharacter.h"
#include "Void/DamageSystem/Components/VHCombatComponent.h"

void UANS_MeleeAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	
	if (!Character) return;
	
	UVHCombatComponent* Combat = Character->FindComponentByClass<UVHCombatComponent>();

	if (!Combat) return;

	Combat->StartMeleeDetection();
}

void UANS_MeleeAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	
	if (!Character) return;
	
	UVHCombatComponent* Combat = Character->FindComponentByClass<UVHCombatComponent>();

	if (!Combat) return;

	Combat->EndMeleeDetection();
}
