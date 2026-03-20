// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_AttackStaminaCost.h"

#include "Void/Character/Player/VHPlayerCharacter.h"
#include "Void/DamageSystem/Components/VHStaminaComponent.h"


void UAnimNotify_AttackStaminaCost::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                           const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	AVHPlayerCharacter* PlayerCharacter = Cast<AVHPlayerCharacter>(MeshComp->GetOwner());
	
	if (!PlayerCharacter) return;
	PlayerCharacter->StaminaComponent->ApplyStaminaCost(PlayerCharacter->AttackCost);
}
