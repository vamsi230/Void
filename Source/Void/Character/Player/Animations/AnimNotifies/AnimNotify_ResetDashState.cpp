// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_ResetDashState.h"
#include "Void/Character/Player/VHPlayerCharacter.h"


void UAnimNotify_ResetDashState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (!MeshComp) return;
	AVHPlayerCharacter* Character = Cast<AVHPlayerCharacter>(MeshComp->GetOwner());
	
	if (!Character) return;
	Character->bisDashing = false;
}
