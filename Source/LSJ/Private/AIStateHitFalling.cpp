// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateHitFalling.h"
#include "CPP_Tekken8CharacterParent.h"
#include "AICharacterAnimInstance.h"
#include "AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
void UAIStateHitFalling::SetAttackInfo ( FAttackInfoInteraction& pAttackInfo )
{
	attackInfoArray.Empty ( );
	attackInfoArray.Add ( pAttackInfo );
}

void UAIStateHitFalling::Enter ( UAICharacterAnimInstance* pAnimInstance )
{
	Super::Enter ( pAnimInstance );

	owner->GetBlackboardComponent ( )->SetValueAsBool ( TEXT ( "IsHit" ) , false ); // 원하는 값을 설정
	//공격 받는 애니메이션 추가
	animInstace->StopAllMontages ( 0.1f );
	if ( attackInfoArray[0].DamagePoint == EDamagePointInteraction::Middle )
		animInstace->PlayHitTopMontage ( );
	else
		animInstace->PlayHitMiddleMontage ( );
	if ( attackInfoArray[0].DamageAmount == 10 )
		//owner->GetCharacterMovement ( )->AddImpulse ( attackInfoArray[0].KnockBackDirection * 100.0f , true );
		owner->LaunchCharacter ( attackInfoArray[0].KnockBackDirection , true , true );
}

void UAIStateHitFalling::Execute ( const float& deltatime )
{

}

void UAIStateHitFalling::Exit ( )
{
	Super::Exit ( );
}
