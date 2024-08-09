// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateWalkForward.h"
#include "AICharacterAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "AICharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Animation/AnimTypes.h"

void UAIStateWalkForward::SetDistance ( float pDistance )
{
	distance = pDistance;
}

void UAIStateWalkForward::Enter (UAICharacterAnimInstance* pAnimInstance )
{
	Super::Enter(pAnimInstance);
	
	if(nullptr == owner->aOpponentPlayer )
		return;
	animInstace->PlayerWalkForwardMontage();
	//animInstace->StateWalkForward(true);
	direction=owner->GetActorForwardVector ( );

	//초기 스피드 설정
	walkSpeed = 100.0f;
	walkDashMinSpeed = 80.0f;
	walkTime = 0;

	//초기 상대를 바라보는 Rotator
	FVector opponentPlayerRotator = owner->aOpponentPlayer->GetMesh()->GetBoneLocation ((TEXT("head")));
	opponentPlayerRotator.Z = owner->GetActorLocation ( ).Z;
	lookPlayerRotator = (opponentPlayerRotator - owner->GetActorLocation ( )).Rotation ( );
	//몽타지 세션의 시작과 끝 시간
	owner->GetCurrentMontage ( )->GetSectionStartAndEndTime ( 0 , startFrame , endFrame );
	TArray<const FAnimNotifyEvent*> notifyEvents;
	//몽타지 노티파이의 끝나는 시간
	owner->GetCurrentMontage ( )->GetAnimNotifies ( startFrame , endFrame , false, notifyEvents );
	for ( const FAnimNotifyEvent* NotifyEvent : notifyEvents )
	{
		endFrame= NotifyEvent->GetTriggerTime ();
	}
}

void UAIStateWalkForward::Execute ( const float& deltatime )
{
	Super::Execute ( deltatime );
	if( nullptr==owner )
		return;
	ToLookTargetRotate ( deltatime );

	//속도가 처음에 크고 끝에는 작아지게 하면 좋을 것 같다.
	float moveSpeed;
	if( walkTime < endFrame*0.6) 
		//점점 증가하고
		moveSpeed = FMath::Lerp ( walkDashMinSpeed , walkSpeed , (walkTime += deltatime)/endFrame );
	else 
		//느려지게
		moveSpeed = FMath::Lerp ( walkSpeed , 0 , (walkTime += deltatime)/endFrame );
	owner->AddMovementInput ( direction * moveSpeed * deltatime);
	//owner->SetActorLocation(direction * walkSpeed * deltatime);
}

void UAIStateWalkForward::Exit ( )
{
	//animInstace->StateWalkForward ( false );
	owner->SetStateIdle();
	Super::Exit ( );
}
//void UAIStateWalkForward::TickComponent ( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction )
//{
//	Super::TickComponent ( DeltaTime , TickType , ThisTickFunction );
//	lookPlayerRotator = UKismetMathLibrary::FindLookAtRotation ( owner->GetActorLocation ( ) , owner->aOpponentPlayer->GetActorLocation ( ) );
//	owner->SetActorRotation ( FMath::RInterpConstantTo ( owner->GetActorRotation ( ) , lookPlayerRotator , DeltaTime ,200.0f ) );
//	if( owner->GetDistanceTo( owner->aOpponentPlayer )<distance)
//		Exit();
//
//	// ...
//}