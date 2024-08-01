// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_CollisionLF.h"
#include "AICharacter.h"
void UAnimNotifyState_CollisionLF::NotifyBegin ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation , float TotalDuration )
{
	Super::NotifyBegin ( MeshComp , Animation , TotalDuration);
	AAICharacter* enemy = Cast<AAICharacter>(MeshComp->GetOwner());
	enemy->OnAttackCollisionLF();
}

void UAnimNotifyState_CollisionLF::NotifyEnd ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation )
{
	Super::NotifyEnd ( MeshComp , Animation);
	AAICharacter* enemy = Cast<AAICharacter> ( MeshComp->GetOwner ( ) );
	enemy->OffAttackCollisionLF ( );
}
