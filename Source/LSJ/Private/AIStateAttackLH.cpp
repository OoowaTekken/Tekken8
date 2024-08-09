// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateAttackLH.h"
#include "AICharacterAnimInstance.h"
#include "CPP_CharacterPaul.h"
#include "AICharacter.h"
#include "Components/SphereComponent.h"
void UAIStateAttackLH::Enter ( UAICharacterAnimInstance* pAnimInstance )
{
	Super::Enter ( pAnimInstance );
	startLocation = owner->GetMesh ( )->GetBoneLocation ( (TEXT ( "head" )) );
	//공격거리 head 기준 174.0f RH
	//공격거리 head 기준 92.0f LH

	animInstace->PlayeAttackLHMontage ( );

	owner->GetCurrentMontage ( )->GetSectionStartAndEndTime ( 0 , startFrame , endFrame );
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

void UAIStateAttackLH::Execute ( const float& deltatime )
{
	ToLookTargetRotate ( deltatime );
	totalTime += deltatime;
	if ( totalTime >= endFrame && !btest )
	{
		GEngine->AddOnScreenDebugMessage ( -1 , 1.f , FColor::Red , FString::Printf ( TEXT ( "range : %f " ) , FVector::Dist ( owner->collisionLH->GetComponentLocation ( ) , startLocation ) ) );
		btest = true;
	}
}

void UAIStateAttackLH::Exit ( )
{
	Super::Exit ( );
}

