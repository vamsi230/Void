// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_ResetAttack.h"

#include "Void/Character/Player/VHPlayerCharacter.h"
#include "Void/DamageSystem/Components/VHCombatComponent.h"

void UAnimNotify_ResetAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	// Same logic as before
	if (MeshComp)
	{
		AVHPlayerCharacter* Character = Cast<AVHPlayerCharacter>(MeshComp->GetOwner());
		if (Character && Character->CombatComponent)
		{
			Character->CombatComponent->ResetAttack();
		}
	}
}
