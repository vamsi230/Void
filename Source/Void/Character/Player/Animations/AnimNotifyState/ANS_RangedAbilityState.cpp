// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_RangedAbilityState.h"

#include "Void/Character/Player/VHPlayerCharacter.h"

void UANS_RangedAbilityState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (!MeshComp) return;
	AVHPlayerCharacter* Character = Cast<AVHPlayerCharacter>(MeshComp->GetOwner());
	
	if (!Character) return;
	
	Character->bCanJump = false;
	Character->bCanDash = false;
}

void UANS_RangedAbilityState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
