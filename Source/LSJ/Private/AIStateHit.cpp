// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateHit.h"
#include "CPP_Tekken8CharacterParent.h"
#include "AICharacterAnimInstance.h"
#include "AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

void UAIStateHit::SetAttackInfo ( FAttackInfoInteraction& pAttackInfo )
{
	attackInfoArray.Empty ( );
	attackInfoArray.Add(pAttackInfo);
}

void UAIStateHit::Enter ( UAICharacterAnimInstance* pAnimInstance )
{
	Super::Enter ( pAnimInstance );

	owner->GetBlackboardComponent()->SetValueAsBool ( TEXT ( "IsHit" ) , false ); // 원하는 값을 설정
	//공격 받는 애니메이션 추가
	animInstace->StopAllMontages ( 0.1f );
	if( attackInfoArray[0].DamagePoint == EDamagePointInteraction::Middle )
		animInstace->PlayHitTopMontage ( );
	else
		animInstace->PlayHitMiddleMontage();
}

void UAIStateHit::Execute ( const float& deltatime )
{
	
}

void UAIStateHit::Exit ( )
{
	Super::Exit();
}