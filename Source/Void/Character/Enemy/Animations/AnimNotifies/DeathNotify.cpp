// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathNotify.h"

#include "Void/Character/Enemy/VHBaseEnemyCharacter.h"

void UDeathNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                          const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	AVHBaseEnemyCharacter* EnemyCharacter = Cast<AVHBaseEnemyCharacter>(MeshComp->GetOwner());
	if (!EnemyCharacter) return;
	EnemyCharacter->Destroy();
}
