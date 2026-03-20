// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_CastState.h"

#include "Void/Character/Player/VHPlayerCharacter.h"
#include "Void/DamageSystem/Components/VHCombatComponent.h"

void UANS_CastState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (!MeshComp) return;
	AVHPlayerCharacter* Character = Cast<AVHPlayerCharacter>(MeshComp->GetOwner());
	
	if (!Character) return;
	
	Character->bIsShooting = true;
	Character->CombatComponent->bCanAttack= false;
	Character->bCanJump = false;
	Character->bCanDash = false;
	
}

void UANS_CastState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (!MeshComp) return;
	AVHPlayerCharacter* Character = Cast<AVHPlayerCharacter>(MeshComp->GetOwner());
	
	if (!Character) return;
	
	Character->bIsShooting = false;
	Character->CombatComponent->bCanAttack= true;
	Character->bCanJump = true;
	Character->bCanDash = true;
}
