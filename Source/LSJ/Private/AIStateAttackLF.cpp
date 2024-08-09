﻿// Fill out your copyright notice in the Description page of Project Settings.
#include "AIStateAttackLF.h"
#include "AICharacterAnimInstance.h"
#include "AICharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "CPP_CharacterPaul.h"
void UAIStateAttackLF::Enter ( UAICharacterAnimInstance* pAnimInstance )
{
	Super::Enter ( pAnimInstance );

	if(attackPoint==EAttackPoint::Lower)
		animInstace->PlayeAttackLowerLFMontage ( );
	else
		animInstace->PlayeAttackLFMontage ( );

	owner->GetCurrentMontage ( )->GetSectionStartAndEndTime ( 0 , startFrame , endFrame );
	startLocation = owner->GetMesh ( )->GetBoneLocation ( (TEXT ( "head" )) );
	startLocation.Z = 0;
	TArray<const FAnimNotifyEvent*> notifyEvents;
	//몽타지 노티파이의 끝나는 시간
	owner->GetCurrentMontage ( )->GetAnimNotifies ( startFrame , endFrame , false , notifyEvents );
	for ( const FAnimNotifyEvent* NotifyEvent : notifyEvents )
	{
		endFrame = NotifyEvent->GetTriggerTime ( );
	}
	totalTime = 0;
	btest = false;
}

void UAIStateAttackLF::Execute ( const float& deltatime )
{
	totalTime += deltatime;
	if ( totalTime >= endFrame && !btest )
	{
		btest = true;
		FVector location =owner->collisionLF->GetComponentLocation ( );
		location.Z = 0;
		GEngine->AddOnScreenDebugMessage ( -1 , 1.f , FColor::Red , FString::Printf ( TEXT ( "range : %f " ) , FVector::Dist ( location , startLocation ) ) );
	}
		
}

void UAIStateAttackLF::Exit ( )
{
	Super::Exit ( );
}
